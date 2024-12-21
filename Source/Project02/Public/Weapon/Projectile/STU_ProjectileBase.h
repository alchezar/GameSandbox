// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STU_ProjectileBase.generated.h"

class UProjectileMovementComponent;
class USphereComponent;
class USTU_WeaponFXComponent;

UCLASS()
class PROJECT02_API ASTU_ProjectileBase : public AActor
{
	GENERATED_BODY()

public:
	ASTU_ProjectileBase();
	UProjectileMovementComponent* GetMovementComponent();
	float GetLifeTime() const;
	void SetShotDirection(const FVector& Direction);

protected:
	virtual void BeginPlay() override;
	AController* GetController() const;

private:
	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile | Components")
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile | Components")
	UProjectileMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile | VFX")
	USTU_WeaponFXComponent* WeaponFXComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile")
	float DamageAmount = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile", meta = (Units = "cm"))
	float ProjectileRadius = 5.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile")
	float ProjectileInitialSpeed = 2000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile", meta = (Units = "s"))
	float LifeTime = 5.f;

private:
	FVector ShotDirection = FVector(1.0, 0.0, 0.0);
};
