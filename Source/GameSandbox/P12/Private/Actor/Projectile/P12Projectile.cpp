// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Projectile/P12Projectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "P12/Public/Actor/Equipment/Throwable/P12ThrowableItem.h"

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

void AP12Projectile::LaunchProjectile(const FVector& Direction,  AActor* LaunchedFrom)
{
	CollisionComponent->IgnoreActorWhenMoving(GetOwner(), true);
	CollisionComponent->IgnoreActorWhenMoving(LaunchedFrom, true);

	// const FRotator ViewRotation = Cast<APawn>(GetOwner())->GetControlRotation();
	// const FVector ViewForwardVector = Direction.GetSafeNormal();
	// const FVector ViewUpVector = ViewRotation.RotateVector(FVector::UpVector);
	// const float Tangent = FMath::Tan(FMath::DegreesToRadians(ThrowableItem->GetGravityCompensationAngle()));
	// const FVector ThrowDirection = ViewForwardVector + ViewUpVector * Tangent;
	//
	// MovementComponent->Velocity = ThrowDirection * Impulse;
	
	// DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + ViewForwardVector * Impulse, 100.f, FColor::Red, false, 5.f);
	// DrawDebugDirectionalArrow(GetWorld(), GetActorLocation(), GetActorLocation() + MovementComponent->Velocity, 100.f, FColor::Green, false, 5.f);
}
