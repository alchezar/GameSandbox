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
#include "Interface/P16CombatInterface.h"
#include "Kismet/GameplayStatics.h"

AP16Projectile::AP16Projectile()
{
	bReplicates     = true;
	InitialLifeSpan = LifeSpan;

	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.TickInterval = 0.f;

	Sphere = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	SetRootComponent(Sphere);
	Sphere->SetCollisionObjectType(P16::CollisionChannel::Projectile);

	ProjectileMovement                         = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	ProjectileMovement->ProjectileGravityScale = 0.f;
	ProjectileMovement->bIsHomingProjectile    = bHoming;
}

void AP16Projectile::BeginPlay()
{
	Super::BeginPlay();
	SetReplicateMovement(true);

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->MaxSpeed     = Speed;

	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlapCallback);

	LoopingSoundComp = UGameplayStatics::SpawnSoundAttached(LoopingSound.Get(), GetRootComponent());

	FTimerHandle TargetCheckTimer;
	GetWorld()->GetTimerManager().SetTimer(TargetCheckTimer, this, &ThisClass::CheckTarget, 1.f, true, 1.f);
}

void AP16Projectile::Tick(const float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AP16Projectile::Destroyed()
{
	PlayOnHitEffects();

	Super::Destroyed();
}

void AP16Projectile::UpdateHomingTarget(const AActor* HomingTarget, const FVector& MouseLocation)
{
	EARLY_RETURN_IF(!bHoming)

	if (HomingTarget && HomingTarget->Implements<UP16CombatInterface>())
	{
		ProjectileMovement->HomingTargetComponent = HomingTarget->GetRootComponent();
	}
	else
	{
		HomingTargetSceneComponent = NewObject<USceneComponent>(this);
		HomingTargetSceneComponent->SetWorldLocation(MouseLocation);
		ProjectileMovement->HomingTargetComponent = HomingTargetSceneComponent;
	}

	constexpr TP16Range AccelerationRange           = {1800.f, 3600.f};
	ProjectileMovement->HomingAccelerationMagnitude = FMath::RandRange(AccelerationRange.Min, AccelerationRange.Max);
	ProjectileMovement->bIsHomingProjectile         = true;
}

void AP16Projectile::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (HasAuthority())
	{
		ApplyDamageTo(OtherActor);
		Destroy();
	}
}

void AP16Projectile::CheckTarget()
{
	EARLY_RETURN_IF(ProjectileMovement->HomingTargetComponent.IsValid())
	Destroy();
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
	EARLY_RETURN_IF(!DamageEffectParams.SourceAbilitySystemComponent)
	const AActor* SourceAvatar = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();

	// Ignore instigator and friends.
	EARLY_RETURN_IF(SourceAvatar == Target)
	EARLY_RETURN_IF(!UP16AbilitySystemLibrary::GetIsNotFriends(Target, DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor()))

	// Causer location for correct knockback and death impulse.
	UP16DamageGameplayAbility::UpdateTricksVelocity(GetActorLocation(), Target, DamageEffectParams);
	EARLY_RETURN_IF(!DamageEffectParams.TargetAbilitySystemComponent)

	UP16AbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
}
