// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12Projectile.generated.h"

class AP12ThrowableItem;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GAMESANDBOX_API AP12Projectile : public AActor
{
	GENERATED_BODY()

public:
	AP12Projectile();
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return MovementComponent; }
	virtual void LaunchProjectile(const FVector& Direction, AActor* LaunchedFrom);

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UProjectileMovementComponent* MovementComponent;
};
