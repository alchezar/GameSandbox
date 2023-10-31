// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12Projectile.generated.h"

class AP12ThrowableItem;
class UProjectileMovementComponent;
class USphereComponent;

DECLARE_MULTICAST_DELEGATE_TwoParams(FP12OnProjectileHitSignature, const FHitResult&, const FVector& /*Direction*/)

UCLASS()
class GAMESANDBOX_API AP12Projectile : public AActor
{
	GENERATED_BODY()

public:
	AP12Projectile();
	FORCEINLINE UProjectileMovementComponent* GetProjectileMovement() const { return MovementComponent; }
	virtual void LaunchProjectile(const FVector& Direction, AActor* LaunchedFrom);

protected:
	virtual void BeginPlay() override;
	
	UFUNCTION()
	void OnCollisionHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	void OnProjectileHitHandle(const FHitResult& HitResult, const FVector& Vector);
	
public:
	FP12OnProjectileHitSignature OnProjectileHit;
	
protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* MeshComponent;
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Component")
	UProjectileMovementComponent* MovementComponent;
};
