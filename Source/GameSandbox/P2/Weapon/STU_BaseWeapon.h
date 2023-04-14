// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameSandbox/P2/STU_CoreTypes.h"
#include "STU_BaseWeapon.generated.h"

class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class GAMESANDBOX_API ASTU_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTU_BaseWeapon();

protected:
	virtual void BeginPlay() override;

#pragma region Shoot

public:
	FOnClipEmptySignature OnClipEmpty;

	virtual void StartFire();
	virtual void StopFire();
	virtual void Aiming();
	virtual bool CanAim() const;

	bool CanReload() const;

protected:
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd) const;
	virtual void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	virtual void MakeDamage(const FHitResult& HitResult);

	APlayerController* GetPlayerController() const;
	FVector            GetMuzzleSocketLocation() const;

	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName SocketName = "MuzzleSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "cm"))
	float TraceMaxDistance = 5000.f;
			
private:
	FTimerHandle ShotTimer;
	bool         bCanAim = true;

#pragma endregion // Shoot

#pragma region Ammo

public:
	virtual void ChangeClip();
	
	FWeaponUIData GetUIData() const;
	FAmmoData     GetAmmoData() const;
	bool          TryToAddAmmo(int32 Clips);

protected:
	virtual void DecreaseAmmo();

	bool IsClipEmpty() const;
	bool IsAmmoEmpty() const;
	bool IsAmmoFull() const;
	void LogAmmo() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FAmmoData DefaultAmmo{15, 10, false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FWeaponUIData UIData;

private:
	FAmmoData CurrentAmmo;

#pragma endregion // Ammo
	
#pragma region VFX

protected:
	UNiagaraComponent* SpawnMuzzleFX() const;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	UNiagaraSystem* MuzzleFX;
	
#pragma endregion // VFX

};
