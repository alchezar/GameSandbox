// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Projectile/P12Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AP12Projectile::AP12Projectile()
{
	SetReplicates(true);
	SetReplicatingMovement(true);
	
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
	MovementComponent->bAutoActivate = false;
}

void AP12Projectile::BeginPlay()
{
	Super::BeginPlay();

	CollisionComponent->OnComponentHit.AddDynamic(this, &ThisClass::OnCollisionHitHandle);
}

void AP12Projectile::LaunchProjectile(const FVector& Direction, AActor* LaunchedFrom)
{
	AActor* TestActor = LaunchedFrom->GetOwner();
	CollisionComponent->IgnoreActorWhenMoving(LaunchedFrom, true);
	CollisionComponent->IgnoreActorWhenMoving(LaunchedFrom->GetOwner(), true);
}

void AP12Projectile::OnCollisionHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnProjectileHit.Broadcast(Hit, MovementComponent->Velocity.GetSafeNormal(), this);
}

void AP12Projectile::ToggleActive(const bool bActive, const FVector& Location, const FVector& Direction, AActor* LaunchedFrom)
{
	MovementComponent->SetActive(bActive);
	MeshComponent->SetVisibility(bActive);
	
	SetActorLocation(Location);
	SetActorRotation(Direction.ToOrientationRotator());

	if (bActive && LaunchedFrom)
	{
		LaunchProjectile(Direction.GetSafeNormal(), LaunchedFrom);
	}
}
