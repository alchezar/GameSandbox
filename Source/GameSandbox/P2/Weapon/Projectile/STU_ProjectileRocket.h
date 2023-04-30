// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_ProjectileBase.h"
#include "STU_ProjectileRocket.generated.h"

UCLASS()
class GAMESANDBOX_API ASTU_ProjectileRocket : public ASTU_ProjectileBase
{
	GENERATED_BODY()

public:
	ASTU_ProjectileRocket();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& HitResult) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile | Rocket", meta = (EditCondition = "bRadiusDamage", Units = "cm"))
	float DamageRadius = 200.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Projectile | Rocket", meta = (EditCondition = "bRadiusDamage"))
	bool bFullDamage = false;
};
