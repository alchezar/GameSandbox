// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseWeapon.h"
#include "STU_LauncherWeapon.generated.h"

class ASTU_Projectile;

UCLASS()
class GAMESANDBOX_API ASTU_LauncherWeapon : public ASTU_BaseWeapon
{
	GENERATED_BODY()

public:
	ASTU_LauncherWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void BeginPlay() override;
	virtual void MakeShot() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	TSubclassOf<ASTU_Projectile> ProjectileClass;

private:
	void DrawProjectilePath();

	bool bAim = false;
};
