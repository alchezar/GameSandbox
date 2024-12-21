// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseWeapon.h"
#include "STU_RifleWeapon.generated.h"

class USTU_WeaponFXComponent;
class UNiagaraSystem;
class UNiagaraComponent;
class UAudioComponent;

UCLASS()
class PROJECT02_API ASTU_RifleWeapon : public ASTU_BaseWeapon
{
	GENERATED_BODY()

public:
	ASTU_RifleWeapon();
	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void BeginPlay() override;
	virtual void MakeShot() override;
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) override;
	virtual void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const override;
	virtual void MakeDamage(const FHitResult& HitResult) override;
	virtual void DecreaseAmmo() override;

private:
	void InitFX();
	void SetFXActive(bool IsActive);
	void SpawnTraceFX(const FVector& TraceStart, const FVector& TraceEnd);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	float DamageAmount = 10.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "deg"))
	float BulletSpread = 3.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "s"))
	float TimeBetweenShots = 0.1f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon FX")
	USTU_WeaponFXComponent* WeaponFXComponent;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	UNiagaraSystem* TraceFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	FName TraceTargetName = "TraceTarget";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	float TaleLength = 200.f;

private:
	UPROPERTY()
	UNiagaraComponent* MuzzleFXComponent;
	FTimerHandle ShotTimer;
	UPROPERTY()
	UAudioComponent* FireAudioComponent;
};
