// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseWeapon.h"
#include "STU_BlasterWeapon.generated.h"

class ASTU_ProjectileBullet;
class USTU_WeaponFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class PROJECT02_API ASTU_BlasterWeapon : public ASTU_BaseWeapon
{
	GENERATED_BODY()

public:
	ASTU_BlasterWeapon();
	virtual void StartFire() override;
	virtual void StopFire() override;
	virtual void ToggleAim(const bool bAim) override;
	FVector GetShootDirection() const;

protected:
	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) override;
	virtual void DecreaseAmmo() override;

private:
	void InitMuzzleFX();
	void SetMuzzleFXVisibility(bool Visible);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	float DamageAmount = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "deg"))
	float BulletSpread = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "s"))
	float TimeBetweenShots = 0.1f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "deg"))
	float ZoomFOV = 50.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	TSubclassOf<ASTU_ProjectileBullet> BulletClass;

private:
	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;
	FTimerHandle ShotTimer;
	FVector ShootDirection;
	float DefaultBulletSpread = 0.f;
	float DefaultCameraFOV    = 90.f;
};
