// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TG_Projectile.generated.h"

class UNiagaraSystem;
class UNiagaraComponent;
class ATG_Gun;
class USphereComponent;
class UProjectileMovementComponent;

UCLASS(config=Game)
class ATG_Projectile : public AActor
{
	GENERATED_BODY()

public:
	ATG_Projectile();
	virtual void Tick(float DeltaSeconds) override;
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

	USphereComponent*             GetCollisionComp() const;
	UProjectileMovementComponent* GetProjectileMovement() const;

	void SetProjectileOwner(ATG_Gun* NewProjectileOwner);

protected:
	virtual void BeginPlay() override;

private:

protected:
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Projectile")
	USphereComponent* CollisionComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Kinder | Movement")
	UProjectileMovementComponent* ProjectileMovement;
	
private:
	UPROPERTY()
	ATG_Gun* ProjectileOwner;
};
