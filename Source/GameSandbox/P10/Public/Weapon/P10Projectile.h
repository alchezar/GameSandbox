// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P10Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GAMESANDBOX_API AP10Projectile : public AActor
{
	GENERATED_BODY()

public:
	AP10Projectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

	void Explode();
	UFUNCTION()
	void OnCollisionHitHandle(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	USphereComponent* CollisionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Component")
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Effect")
	UParticleSystem* ExplosionFX;
	
};
