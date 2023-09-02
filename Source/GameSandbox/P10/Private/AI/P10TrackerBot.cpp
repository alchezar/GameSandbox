// Copyright (C) 2023, IKinder

#include "P10/Public/AI/P10TrackerBot.h"

#include "DrawDebugHelpers.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "P10/Public/Component/P10HealthComponent.h" 
#include "P10/Public/Util/P10Library.h"
#include "Particles/ParticleSystem.h"

AP10TrackerBot::AP10TrackerBot()
{
	PrimaryActorTick.bCanEverTick = true;
	SetReplicates(true);
	SetReplicatingMovement(true);

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCanEverAffectNavigation(false);
	MeshComp->SetSimulatePhysics(true);
	MeshComp->SetIsReplicated(true);

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SphereComp->SetupAttachment(RootComponent);
	SphereComp->SetSphereRadius(DamageRadius);
	SphereComp->SetGenerateOverlapEvents(true);
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);

	HealthComp = CreateDefaultSubobject<UP10HealthComponent>("HealthComponent");

	FindDefaultReferences();
}

void AP10TrackerBot::BeginPlay()
{
	Super::BeginPlay();
	check(HealthComp)
	check(MeshComp)
	
	SphereComp->AttachToComponent(MeshComp, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, EAttachmentRule::KeepWorld, false));
	if (HasAuthority())
	{
		HealthComp->OnHealthChanged.AddUObject(this, &ThisClass::OnHealthChangedHandle);
		SphereComp->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlapHandle);
		FindNextPathPoint(TargetPawn);
	}
	TargetPawn = UGameplayStatics::GetPlayerPawn(this, 0);
	LastFrameLocation = MeshComp->GetComponentLocation();
	CreateRollingAudioComponent();
}

void AP10TrackerBot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bDead) return;
	
	if (HasAuthority())
	{
		MoveToNextPoint();
	}
	PlayRollingSound();
}

void AP10TrackerBot::FindNextPathPoint(AActor* Goal)
{
	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, MeshComp->GetComponentLocation(), Goal);
	if (!NavPath || NavPath->PathPoints.Num() < 1)
	{
		NextPathPoint = MeshComp->GetComponentLocation();
		return;
	}

	const FVector NextPoint = NavPath->PathPoints[1];
	NextPathPoint = NextPoint;
}

void AP10TrackerBot::MoveToNextPoint()
{
	/* Check the distance to the target, if we are close enough to it - find the next target. */
	const FVector CurrentLocation = MeshComp->GetComponentLocation();
	const float DistanceToTarget = FVector::Dist(NextPathPoint, CurrentLocation);
	if (DistanceToTarget < AcceptableDistance)
	{
		UGameplayStatics::PlaySoundAtLocation(this, Sound.TargetReached, NextPathPoint);
		FindNextPathPoint(TargetPawn);
		return;
	}

	/* Find the angle between current move and target directions, which will proportionally affect the miss compensation force. */
	const FVector MoveDirection = (CurrentLocation - LastFrameLocation).GetSafeNormal2D();
	const FVector TargetDirection = (NextPathPoint - CurrentLocation).GetSafeNormal2D();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(MoveDirection, TargetDirection)));
	const float MissCompensation = FMath::GetMappedRangeValueClamped(FVector2D(0.f ,180.f), FVector2D(1.f, MaxMissCompensation), Angle);

	/* To correct the motion force - invert current move direction if we`ve already missed the target, or find reflection vector if we haven`t. */
	const FVector ResistDirection = (Angle > 90.f) ? (-1.f * MoveDirection) : (FMath::GetReflectionVector(-1.f * MoveDirection, TargetDirection));
	const FVector FinalDirection = (TargetDirection + ResistDirection).GetSafeNormal2D();

	MeshComp->AddForce(FinalDirection * MissCompensation * Force, NAME_None, bVelocityChange);
	LastFrameLocation = CurrentLocation;

	if (UP10Library::GetIsDrawDebugAllowed() && GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, -1.f, FColor::Yellow, FString::Printf(TEXT("Miss compensation: %f"), MissCompensation));
		DrawDebugCapsule(GetWorld(), NextPathPoint, 20.f, 20.f, FQuat::Identity, FColor::Blue, false, -1.f);
		DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + MoveDirection   * 150.f, 10.f, FColor::Red,    false, -1, 0, 5.f);
		DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + TargetDirection * 150.f, 10.f, FColor::Blue,   false, -1, 0, 5.f);
		DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + ResistDirection * 150.f, 10.f, FColor::Yellow, false, -1, 0, 5.f);
		DrawDebugDirectionalArrow(GetWorld(), CurrentLocation, CurrentLocation + FinalDirection  * 150.f, 10.f, FColor::Green,  false, -1, 0, 5.f);
	}
}

void AP10TrackerBot::OnHealthChangedHandle(UP10HealthComponent* Component, float Health, float Delta, const UDamageType* DamageType, AController* InstignatedBy, AActor* DamageCauser)
{
	/* Explode if dead. */
	if (Health <= 0)
	{
		Suicide();
	}

	/* Pulse material on hit */
	FTimerDelegate PulseDelegate;
	PulseDelegate.BindUObject(this, &ThisClass::PulseOnHitHandle, MeshComp->CreateAndSetMaterialInstanceDynamic(0));
	GetWorld()->GetTimerManager().SetTimer(PulseTimer, PulseDelegate, GetWorld()->GetDeltaSeconds(), true);
}

void AP10TrackerBot::PulseOnHitHandle(UMaterialInstanceDynamic* PulseMaterial)
{
	Alpha += GetWorld()->GetDeltaSeconds() / PulseTime;
	if (Alpha > 2.f)
	{
		GetWorld()->GetTimerManager().ClearTimer(PulseTimer);
		Alpha = 0.f;
		return;
	}	
	const float Pi = 2 * acos(0.f);
	const float SinAlpha = 0.5 - cos(Alpha * Pi) / 2.f;
	
	PulseMaterial->SetVectorParameterValue("Emission", FMath::Lerp(FLinearColor::Black, FLinearColor::Red * 10, SinAlpha));
}

void AP10TrackerBot::OnSphereBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == this || !Cast<APawn>(OtherActor))	return;

	FTimerHandle KamikazeTimer;
	GetWorld()->GetTimerManager().SetTimer(KamikazeTimer, this, &ThisClass::Suicide, SelfDamageInterval, false);
	UGameplayStatics::SpawnSoundAtLocation(this, Sound.SelfDestruct,MeshComp->GetComponentLocation());
}

void AP10TrackerBot::Suicide()
{
	const TArray<AActor*> IgnoredActors = {this};
	UGameplayStatics::SpawnEmitterAtLocation(this, Explosion, MeshComp->GetComponentLocation(), FRotator::ZeroRotator, FVector(2.f));
	UGameplayStatics::SpawnSoundAtLocation(this, Sound.Explode, MeshComp->GetComponentLocation());
	// TODO: Explosion particles and sound spawns only on server now =(
	if (AudioComponent)
	{
		AudioComponent->Stop();
	}
	MeshComp->SetVisibility(false);
	bDead = true;

	UGameplayStatics::ApplyRadialDamage(this, Damage, MeshComp->GetComponentLocation(), DamageRadius, nullptr, IgnoredActors, this, nullptr, true);
	if (UP10Library::GetIsDrawDebugAllowed())
	{
		UP10Library::DrawDebugExplode(this, MeshComp->GetComponentLocation(), DamageRadius);
	}
	SphereComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	SetLifeSpan(2.f);
}

void AP10TrackerBot::FindDefaultReferences()
{
	if (!Sound.Explode)
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> ExplodeSoundFinder(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Project/PP10/Sound/MFX_Explode.MFX_Explode'"));
		if (ExplodeSoundFinder.Succeeded()) Sound.Explode = ExplodeSoundFinder.Object;
	}
	if (!Sound.Rolling)
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> RollingSoundFinder(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Project/PP10/Sound/MFX_Rolling.MFX_Rolling'"));
		if (RollingSoundFinder.Succeeded()) Sound.Rolling = RollingSoundFinder.Object;
	}
	if (!Sound.TargetReached)
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> TargetReachedSoundFinder(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Project/PP10/Sound/MFX_Reached.MFX_Reached'"));
		if (TargetReachedSoundFinder.Succeeded()) Sound.TargetReached = TargetReachedSoundFinder.Object;
	}
	if (!Explosion)
	{
		static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionFinder(TEXT("/Script/Engine.ParticleSystem'/Game/StarterContent/Particles/P_Explosion.P_Explosion'"));
		if (ExplosionFinder.Succeeded()) Explosion = ExplosionFinder.Object;
	}
	if (!Sound.SelfDestruct)
	{
		static ConstructorHelpers::FObjectFinder<USoundBase> SelfDestructSoundFinder(TEXT("/Script/MetasoundEngine.MetaSoundSource'/Game/Project/PP10/Sound/MFX_SelfDestruct.MFX_SelfDestruct'"));
		if (SelfDestructSoundFinder.Succeeded()) Sound.SelfDestruct = SelfDestructSoundFinder.Object;
	}
}

void AP10TrackerBot::CreateRollingAudioComponent()
{
	if (AudioComponent) return;
	check(Sound.Rolling)
	AudioComponent = UGameplayStatics::SpawnSoundAttached(Sound.Rolling, MeshComp);
	if (!AudioComponent) return;

	AudioComponent->bOverrideAttenuation = true;
	AudioComponent->AttenuationOverrides.DistanceAlgorithm = EAttenuationDistanceModel::NaturalSound;
	AudioComponent->AttenuationOverrides.AttenuationShape = EAttenuationShape::Sphere;
	AudioComponent->AttenuationOverrides.FalloffMode = ENaturalSoundFalloffMode::Silent;
	AudioComponent->AttenuationOverrides.AttenuationShapeExtents = FVector(200.f);
	AudioComponent->AttenuationOverrides.FalloffDistance = 1000.f;
}

void AP10TrackerBot::PlayRollingSound()
{
	if (!AudioComponent) return;
	
	/* Sound volume depends of component`s speed. */
	const float CurrentVelocity = MeshComp->GetComponentVelocity().Size();
	const float VolumeMultiplier = FMath::GetMappedRangeValueClamped(FVector2D(0.f, 600.f), FVector2D(0.01f, 1.f), CurrentVelocity);
	AudioComponent->SetVolumeMultiplier(VolumeMultiplier);
	
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("%f"), CurrentVelocity));
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Cyan, FString::Printf(TEXT("%f"), VolumeMultiplier));
}

void AP10TrackerBot::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, NextPathPoint)
}
