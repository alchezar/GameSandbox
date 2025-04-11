// Copyright © 2025, Ivan Kinder

#include "Actor/P16Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AP16Projectile::AP16Projectile()
{
	bReplicates = true;

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

	SetLifeSpan(LifeSpan);
	Sphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnSphereBeginOverlapCallback);

	LoopingSoundComp = UGameplayStatics::SpawnSoundAttached(LoopingSound.Get(), GetRootComponent());
}

void AP16Projectile::Destroyed()
{
	if (!bHit && !HasAuthority())
	{
		PlayOnHitEffects();
	}

	Super::Destroyed();
}

void AP16Projectile::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	EARLY_RETURN_IF(!DamageEffectParams.SourceAbilitySystemComponent)
	const AActor* SourceAvatar = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	EARLY_RETURN_IF(SourceAvatar == OtherActor)

	if (!bHit)
	{
		PlayOnHitEffects();
	}

	if (HasAuthority())
	{
		ApplyDamageTo(OtherActor);
		Destroy();
	}
}

void AP16Projectile::PlayOnHitEffects()
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound.Get(), GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect.Get(), GetActorLocation());

	if (LoopingSoundComp)
	{
		LoopingSoundComp->Stop();
	}
	bHit = true;
}

void AP16Projectile::ApplyDamageTo(AActor* Target)
{
	EARLY_RETURN_IF(!UP16AbilitySystemLibrary::GetIsNotFriends(Target, DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor()))

	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	EARLY_RETURN_IF(!DamageEffectParams.TargetAbilitySystemComponent)

	UP16AbilitySystemLibrary::ApplyDamageEffect(DamageEffectParams);
}
