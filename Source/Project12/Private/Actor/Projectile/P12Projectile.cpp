// Copyright (C) 2023, IKinder

#include "Actor/Projectile/P12Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AP12Projectile::AP12Projectile()
{
	PrimaryActorTick.bCanEverTick = true;
	// SetReplicates(true);
	bReplicates = true;
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

void AP12Projectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (!IsHidden())
	{
		if (GetActorLocation() == LastFrameLocation)
		{
			ToggleActive(false, PoolLocation);
		}
		LastFrameLocation = GetActorLocation();
	}
}

void AP12Projectile::LaunchProjectile(const FVector& Direction, AActor* LaunchedFrom)
{
	CollisionComponent->IgnoreActorWhenMoving(LaunchedFrom, true);
	CollisionComponent->IgnoreActorWhenMoving(LaunchedFrom->GetOwner(), true);

	MovementComponent->Velocity = Direction * MovementComponent->InitialSpeed;
}

void AP12Projectile::OnCollisionHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	OnProjectileHit.Broadcast(Hit, MovementComponent->Velocity.GetSafeNormal(), this);
}

void AP12Projectile::ToggleActive(const bool bActive, const FVector& Location, const FVector& Direction, AActor* WeaponOwner)
{
	/* Save PoolLocation on Projectile initialization. */
	if (!bActive && PoolLocation == FVector::ZeroVector)
	{
		PoolLocation = Location;
	}

	SetActorLocation(Location);
	SetActorRotation(Direction.ToOrientationRotator());

	if (bActive && WeaponOwner)
	{
		LaunchProjectile(Direction.GetSafeNormal(), WeaponOwner);
	}

	/* Set projectile active */
	SetHidden(!bActive);
	MeshComponent->SetVisibility(bActive);
	MovementComponent->SetActive(bActive, bActive);
}
