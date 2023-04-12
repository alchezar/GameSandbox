// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSandbox/P2/STU_CoreTypes.h"
#include "STU_BaseWeapon.generated.h"

class USkeletalMeshComponent;

UCLASS()
class GAMESANDBOX_API ASTU_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTU_BaseWeapon();

	FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire();
	virtual void StopFire();
	virtual void Aiming();

	virtual void ChangeClip();
	virtual bool CanAim() const;
	bool         CanReload() const;

protected:
	virtual void BeginPlay() override;

	virtual void       MakeShot();
	APlayerController* GetPlayerController() const;
	FVector            GetMuzzleSocketLocation() const;
	virtual bool       GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	bool               GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	virtual void       MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	virtual void       MakeDamage(const FHitResult& HitResult);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName SocketName = "MuzzleSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "cm"))
	float TraceMaxDistance = 5000.f;

private:
	FTimerHandle ShotTimer;
	bool         bCanAim = true;

#pragma region Ammo

public:
	FWeaponUIData GetUIData() const;
	FAmmoData     GetAmmoData() const;
	bool          TryToAddAmmo(int32 Clips);

protected:
	virtual void DecreaseAmmo();
	bool         IsClipEmpty() const;
	bool         IsAmmoEmpty() const;
	bool         IsAmmoFull() const;
	void         LogAmmo() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FAmmoData DefaultAmmo{15, 10, false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FWeaponUIData UIData;

private:
	FAmmoData CurrentAmmo;

#pragma endregion // Ammo
};
