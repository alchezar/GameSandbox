// Copyright © 2025, Ivan Kinder

#include "Actor/P16Projectile.h"

#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "AbilitySystem/Ability/P16DamageGameplayAbility.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AP16Projectile::AP16Projectile()
{
	bReplicates     = true;
	InitialLifeSpan = LifeSpan;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.f;

	Sphere = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(P16::Gecc_Projectile);

	ProjectileMovement                         = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovement->InitialSpeed           = Speed;
	ProjectileMovement->MaxSpeed               = Speed;
	ProjectileMovement->ProjectileGravityScale = 0.f;
}

void AP16Projectile::BeginPlay()
{
	Super::BeginPlay();

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlapCallback);

	LoopingSoundComp = UGameplayStatics::SpawnSoundAttached(LoopingSound.Get(), GetRootComponent());

	// Switch between projectile movement and custom floating movement.
	LastTickLocation = GetActorLocation();
	ForwardDirection = GetActorForwardVector();
	if (DistanceToGround > 0.f)
	{
		ProjectileMovement->bSimulationEnabled = false;
		ProjectileMovement->InitialSpeed       = 0.f;
	}
}

void AP16Projectile::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	CustomMove(DeltaSeconds);
}

void AP16Projectile::Destroyed()
{
	PlayOnHitEffects();

	Super::Destroyed();
}

void AP16Projectile::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EARLY_RETURN_IF(!DamageEffectParams.SourceAbilitySystemComponent)
	const AActor* SourceAvatar = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	EARLY_RETURN_IF(SourceAvatar == OtherActor)

	if (HasAuthority())
	{
		ApplyDamageTo(OtherActor);
		Destroy();
	}
}

void AP16Projectile::PlayOnHitEffects() const
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound.Get(), GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect.Get(), GetActorLocation());

	if (LoopingSoundComp)
	{
		LoopingSoundComp->Stop();
	}
}

void AP16Projectile::ApplyDamageTo(AActor* Target)
{
	EARLY_RETURN_IF(!UP16AbilitySystemLibrary::GetIsNotFriends(Target, DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor()))

	UP16DamageGameplayAbility::UpdateDamageEffectParams(Target, DamageEffectParams);
	EARLY_RETURN_IF(!DamageEffectParams.TargetAbilitySystemComponent)

	UP16AbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
}

void AP16Projectile::CustomMove(const float DeltaSeconds)
{
	EARLY_RETURN_IF(FMath::IsNearlyZero(DistanceToGround))

	FHitResult            Hit;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(GetInstigator());

	GetWorld()->LineTraceSingleByChannel(
		Hit,
		GetActorLocation(),
		GetActorLocation() - FVector(0.f, 0.f, 10000.f),
		ECC_Camera,
		Params);

	// Up vector.
	FVector ResultLocation = Hit.bBlockingHit ? Hit.ImpactPoint + FVector::UpVector * DistanceToGround : LastTickLocation;
	// Forward vector.
	ResultLocation += ForwardDirection * ProjectileMovement->GetMaxSpeed() * DeltaSeconds;

	// Move.
	SetActorLocation(ResultLocation);
	SetActorRotation((ResultLocation - LastTickLocation).Rotation());
	LastTickLocation = ResultLocation;
}
