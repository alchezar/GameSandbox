// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "STU_Projectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

UCLASS()
class GAMESANDBOX_API ASTU_Projectile : public AActor
{
	GENERATED_BODY()

public:
	ASTU_Projectile();

	void                          SetShotDirection(const FVector& Direction);
	UProjectileMovementComponent* GetMovementComponent() const;
	float                         GetLifeTime() const;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	USphereComponent* CollisionComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	UProjectileMovementComponent* MovementComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "cm"))
	float DamageRadius = 200.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	float DamageAmount = 50.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	bool bFullDamage = false;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "s"))
	float LifeTime = 5.f;

private:
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	AController* GetController() const;

	FVector ShotDirection;
};
