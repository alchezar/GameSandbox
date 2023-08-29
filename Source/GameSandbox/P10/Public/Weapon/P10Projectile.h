// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10Projectile.generated.h"

class AP10Weapon;
class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GAMESANDBOX_API AP10Projectile : public AActor
{
	GENERATED_BODY()

public:
	AP10Projectile();
	virtual void Tick(float DeltaTime) override;
	// FORCEINLINE void SetLauncher(AP10Weapon* LauncherWeapon) { Launcher = LauncherWeapon; }

protected:
	virtual void BeginPlay() override;

	void Explode(const FHitResult& Hit);
	UFUNCTION()
	void OnCollisionHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* CollisionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UStaticMeshComponent* ProjectileMesh;
};
