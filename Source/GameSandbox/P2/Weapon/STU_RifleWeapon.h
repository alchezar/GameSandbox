// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_BaseWeapon.h"
#include "STU_RifleWeapon.generated.h"

UCLASS()
class GAMESANDBOX_API ASTU_RifleWeapon : public ASTU_BaseWeapon
{
	GENERATED_BODY()

public:
	ASTU_RifleWeapon();

	virtual void StartFire() override;
	virtual void StopFire() override;

protected:
	virtual void BeginPlay() override;
	virtual void MakeShot() override;

	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const override;
	virtual void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const override;
	virtual void MakeDamage(const FHitResult& HitResult) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	float DamageAmount = 10.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "deg"))
	float BulletSpread = 3.f;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "s"))
	float TimeBetweenShots = 0.1f;

private:
	FTimerHandle ShotTimer;
};
