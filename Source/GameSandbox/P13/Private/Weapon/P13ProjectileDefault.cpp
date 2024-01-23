// Copyright © 2024, IKinder

#include "P13/Public/Weapon/P13ProjectileDefault.h"

#include "GameSandbox.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "P13/Public/Intearface/P13StateEffectInterface.h"
#include "Particles/ParticleSystemComponent.h"
#include "Perception/AISense_Damage.h"

AP13ProjectileDefault::AP13ProjectileDefault()
{
	PrimaryActorTick.bCanEverTick = true;

	BulletCollision = CreateDefaultSubobject<USphereComponent>("BulletCollisionSphereComponent");
	SetRootComponent(BulletCollision);
	BulletCollision->SetSphereRadius(5.f);
	BulletCollision->bReturnMaterialOnMove = true;      // Hit event return physical material
	BulletCollision->SetCanEverAffectNavigation(false); // Collision not affect navigation (P keyboard in editor)

	BulletMesh = CreateDefaultSubobject<UStaticMeshComponent>("BulletStaticMesh");
	BulletMesh->SetupAttachment(RootComponent);
	BulletMesh->SetCanEverAffectNavigation(false);

	BulletTrace = CreateDefaultSubobject<UParticleSystemComponent>("BulletTraceParticleSystem");
	BulletTrace->SetupAttachment(RootComponent);

	BulletMovement = CreateDefaultSubobject<UProjectileMovementComponent>("BulletProjectileMovementComponent");
	BulletMovement->UpdatedComponent = RootComponent;
	BulletMovement->InitialSpeed = 1.f;
	BulletMovement->MaxSpeed = 0.f;
	BulletMovement->bRotationFollowsVelocity = true;
	BulletMovement->bShouldBounce = true;
}

void AP13ProjectileDefault::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	BulletCollision->OnComponentHit.AddDynamic(this, &ThisClass::OnBulletHitHandle);
	BulletCollision->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnBulletBeginOverlapHandle);
	BulletCollision->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnBulletEndOverlapHandle);
}

void AP13ProjectileDefault::BeginPlay()
{
	Super::BeginPlay();

	if (DamageType == EP13ProjectileDamageType::Grenade)
	{
		GetWorld()->GetTimerManager().SetTimer(ExplodeTimer, this, &ThisClass::GiveRadialDamage, BulletSettings.Lifetime);
	}
}

void AP13ProjectileDefault::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP13ProjectileDefault::InitBullet(const float NewLifeSpan, const FP13ProjectileInfo& NewBulletSettings)
{
	InitialLifeSpan = NewLifeSpan;
	BulletMovement->InitialSpeed = NewBulletSettings.InitSpeed;
	BulletSettings = NewBulletSettings;
}

void AP13ProjectileDefault::OnBulletHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if (!OtherActor)
	{
		return;
	}

	SpawnStateEffect(Hit, GetInstigatorController());
	SpawnEffectsOnHit(Hit);

	if (DamageType == EP13ProjectileDamageType::Point)
	{
		GivePointDamage(Hit, OtherActor);
	}
	else if (DamageType == EP13ProjectileDamageType::Radial)
	{
		GiveRadialDamage();
		return;
	}
	else if (DamageType == EP13ProjectileDamageType::Grenade)
	{
		return;
	}
	
	ImpactProjectile();
}

void AP13ProjectileDefault::OnBulletBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

void AP13ProjectileDefault::OnBulletEndOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex)
{
	
}

void AP13ProjectileDefault::SpawnEffectsOnHit(const FHitResult& Hit)
{
	if (!Hit.PhysMaterial.IsValid())
	{
		return;
	}
	const EPhysicalSurface SurfaceType = Hit.PhysMaterial->SurfaceType;
	const FVector& Location = Hit.ImpactPoint;
	const FRotator& Rotation = Hit.ImpactNormal.Rotation();

	if (BulletSettings.OnHit.Decals.Contains(SurfaceType))
	{
		UMaterialInterface* Decal = BulletSettings.OnHit.Decals[SurfaceType];
		UGameplayStatics::SpawnDecalAttached(Decal, FVector(10.f), Hit.GetComponent(), NAME_None, Location, Rotation, EAttachLocation::KeepWorldPosition, 10.f);
	}
	if (BulletSettings.OnHit.Particles.Contains(SurfaceType))
	{
		UNiagaraSystem* Particle = BulletSettings.OnHit.Particles[SurfaceType];
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, Particle, Location, Rotation);
	}
	if (BulletSettings.OnHit.Sounds.Contains(SurfaceType))
	{
		USoundBase* Sound = BulletSettings.OnHit.Sounds[SurfaceType];
		UGameplayStatics::SpawnSoundAtLocation(this, Sound, Location, FRotator::ZeroRotator, 2.f);
	}
}

void AP13ProjectileDefault::ImpactProjectile()
{
	UGameplayStatics::SpawnEmitterAtLocation(this, ExplodeEffect, GetActorLocation(), GetActorRotation());
	Destroy();
}

void AP13ProjectileDefault::GivePointDamage(const FHitResult& Hit, AActor* OtherActor) 
{
	FPointDamageEvent PointDamage;
	PointDamage.HitInfo = Hit;
	PointDamage.ShotDirection = GetActorForwardVector();
	OtherActor->TakeDamage(BulletSettings.Damage, PointDamage, GetInstigatorController(), this);
	
	UAISense_Damage::ReportDamageEvent(GetWorld(), OtherActor, GetInstigator(), BulletSettings.Damage, Hit.Location, Hit.ImpactPoint);
}

void AP13ProjectileDefault::GiveRadialDamage() 
{
	UGameplayStatics::ApplyRadialDamageWithFalloff(GetWorld(), BulletSettings.Damage, 0.f, GetActorLocation(), BulletSettings.DamageRadius / 2.f , BulletSettings.DamageRadius, 1.f, UDamageType::StaticClass(), {this}, this, GetInstigatorController(), ECC_BULLET);
	
	DrawDebugCapsule(GetWorld(), GetActorLocation(), BulletSettings.DamageRadius, BulletSettings.DamageRadius, FQuat::Identity, FColor::Red);
	ImpactProjectile();
}

void AP13ProjectileDefault::SpawnStateEffect(const FHitResult& Hit, AController* Causer)
{
	const IP13StateEffectInterface* StateEffectInterface = Cast<IP13StateEffectInterface>(Hit.GetActor());
	if (!StateEffectInterface)
	{
		return;
	}
	if (!StateEffectInterface->GetCanApplyStateEffect(BulletSettings.StateEffectClass))
	{
		return;
	}
	
	UP13Types::AddEffectBySurfaceType(Hit.GetActor(), BulletSettings.StateEffectClass, Hit.PhysMaterial->SurfaceType, Causer);
}
