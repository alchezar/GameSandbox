// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12Projectile.h"
#include "P12ExplosiveProjectile.generated.h"

class UP12ExplosionComponent;

UCLASS()
class PROJECT12_API AP12ExplosiveProjectile : public AP12Projectile
{
	GENERATED_BODY()

public:
	AP12ExplosiveProjectile();
	virtual void LaunchProjectile(const FVector& Direction, AActor* LaunchedFrom) override;

protected:
	void Detonate();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = "C++ | Explosion")
	UP12ExplosionComponent* ExplosionComponent;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Explosion")
	float DetonationTime = 2.f;

private:
	FTimerHandle DetonationHandle;
};
