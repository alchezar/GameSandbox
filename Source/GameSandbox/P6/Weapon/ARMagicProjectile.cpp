// Copyright (C) 2023, IKinder

#include "ARMagicProjectile.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AARMagicProjectile::AARMagicProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComp = CreateDefaultSubobject<USphereComponent>("SphereComponent");
	SetRootComponent(SphereComp);
	SphereComp->SetCollisionProfileName("Projectile");

	EffectComp = CreateDefaultSubobject<UParticleSystemComponent>("EffectComponent");
	EffectComp->SetupAttachment(RootComponent);

	MovementComp = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComp->InitialSpeed = 1000.f;
	MovementComp->bRotationFollowsVelocity = true;
	MovementComp->bInitialVelocityInLocalSpace = true;
	MovementComp->ProjectileGravityScale = 0.f;
}

void AARMagicProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AARMagicProjectile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARMagicProjectile::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	SphereComp->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHit);
}

void AARMagicProjectile::AddActorToIgnore(AActor* Actor)
{
	SphereComp->IgnoreActorWhenMoving(Actor, true);
}

void AARMagicProjectile::OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	const FString DebugString = FString::Printf(TEXT("Hit location: %s"), *(Hit.ImpactPoint.ToString()));
	DrawDebugString(GetWorld(), Hit.ImpactPoint, DebugString, nullptr, FColor::Green, 5.f);
}
