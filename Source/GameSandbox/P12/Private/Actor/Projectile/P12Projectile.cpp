// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Projectile/P12Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
// #include "P12/Public/Actor/Equipment/Throwable/P12ThrowableItem.h"

AP12Projectile::AP12Projectile()
{
	CollisionComponent = CreateDefaultSubobject<USphereComponent>("SphereCollisionComponent");
	CollisionComponent->InitSphereRadius(5.f);
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionComponent->SetCollisionResponseToAllChannels(ECR_Block);
	SetRootComponent(CollisionComponent);

	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("ProjectileStaticMeshComponent");
	MeshComponent->SetupAttachment(RootComponent);

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	MovementComponent->InitialSpeed = 1000.f;
	MovementComponent->bShouldBounce = true;
	MovementComponent->Friction = 0.5f;
}

void AP12Projectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnCollisionHitHandle);
	OnProjectileHit.AddUObject(this, &ThisClass::OnProjectileHitHandle);
}

void AP12Projectile::LaunchProjectile(const FVector& Direction,  AActor* LaunchedFrom)
{
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->IgnoreActorWhenMoving(LaunchedFrom, true);
}

void AP12Projectile::OnCollisionHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnProjectileHit.Broadcast(Hit, MovementComponent->Velocity.GetSafeNormal());	
}

void AP12Projectile::OnProjectileHitHandle(const FHitResult& HitResult, const FVector& Vector)
{
	MeshComponent->SetVisibility(false);
	SetLifeSpan(2.f);
}
