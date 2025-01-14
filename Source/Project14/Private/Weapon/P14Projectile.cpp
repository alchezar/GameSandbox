// Copyright © 2025, Ivan Kinder

#include "Weapon/P14Projectile.h"

#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"

class UNiagaraComponent;

AP14Projectile::AP14Projectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("CollisionSphereComponent");
	check(CollisionComponent)
	SetRootComponent(CollisionComponent.Get());
	CollisionComponent->InitSphereRadius(10.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("MeshComponent");
	MeshComponent->SetupAttachment(CollisionComponent);

	MovementComponent                         = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed           = 2000.f;
	MovementComponent->ProjectileGravityScale = 0.f;
}

void AP14Projectile::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->Velocity = ShotDirection * MovementComponent->InitialSpeed;
	CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnProjectileHitCallback);
	SetLifeSpan(LifeSeconds);
}

void AP14Projectile::OnProjectileHitCallback(UPrimitiveComponent*, AActor* OtherActor, UPrimitiveComponent*, FVector, const FHitResult& Hit)
{
	if (!GetWorld() || !OtherActor)
	{
		return;
	}

	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Particles.Get(), GetActorLocation(), Hit.ImpactNormal.Rotation());
	MovementComponent->StopMovementImmediately();
	OtherActor->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, this);
	Destroy();
}
