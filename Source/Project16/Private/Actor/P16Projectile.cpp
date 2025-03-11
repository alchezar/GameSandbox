// Copyright © 2025, Ivan Kinder

#include "Actor/P16Projectile.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Project16.h"
#include "Components/AudioComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"

AP16Projectile::AP16Projectile()
{
	SetReplicates(true);

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
		PlayEffects();
	}

	Super::Destroyed();
}

void AP16Projectile::OnSphereBeginOverlapCallback(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	PlayEffects();

	if (HasAuthority())
	{
		ApplyDamageTo(OtherActor);
		Destroy();
	}
	else
	{
		bHit = true;
	}
}

void AP16Projectile::PlayEffects() const
{
	UGameplayStatics::PlaySoundAtLocation(this, ImpactSound.Get(), GetActorLocation());
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(this, ImpactEffect.Get(), GetActorLocation());

	LoopingSoundComp->Stop();
}

void AP16Projectile::ApplyDamageTo(AActor* Target) const
{
	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Target);
	EARLY_RETURN_IF(!TargetAbilitySystem)

	TargetAbilitySystem->ApplyGameplayEffectSpecToSelf(*DamageEffectSpecHandle.Data.Get());
}
