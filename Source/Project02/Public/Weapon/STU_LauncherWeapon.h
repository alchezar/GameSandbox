// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseWeapon.h"
#include "STU_LauncherWeapon.generated.h"

class ASTU_Projectile;

UCLASS()
class PROJECT02_API ASTU_LauncherWeapon : public ASTU_BaseWeapon
{
	GENERATED_BODY()

public:
	ASTU_LauncherWeapon();
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void Aiming() override;
	virtual void ChangeClip() override;

protected:
	virtual void BeginPlay() override;
	virtual void MakeShot() override;

private:
	void DrawProjectilePath();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	TSubclassOf<ASTU_Projectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Sound")
	USoundCue* NoAmmoSound;
};
