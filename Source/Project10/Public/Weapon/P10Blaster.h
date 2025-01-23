// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P10Weapon.h"
#include "P10Blaster.generated.h"

class AP10Projectile;

UCLASS(HideCategories=("C++ | Beam"))
class PROJECT10_API AP10Blaster : public AP10Weapon
{
	GENERATED_BODY()

public:
	AP10Blaster();
	virtual void Tick(float DeltaTime) override;
	virtual void OneShot() override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Spawn")
	TSubclassOf<AP10Projectile> ProjectileClass;
};
