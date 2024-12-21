// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Game/STU_CoreTypes.h"
#include "GameFramework/Actor.h"
#include "STU_BaseWeapon.generated.h"

class USoundCue;
class USkeletalMeshComponent;
class UNiagaraSystem;
class UNiagaraComponent;

UCLASS()
class PROJECT02_API ASTU_BaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ASTU_BaseWeapon();

	FOnClipEmptySignature OnClipEmpty;
	// Shoot
	virtual void StartFire();
	virtual void StopFire();
	virtual void Aiming();
	virtual void ToggleAim(const bool bAim);
	virtual bool GetCanAim() const;
	bool CanReload() const;
	FRotator GetWeaponSocketRotation() const;
	void SetCanAim(const bool bAim);
	// Ammo
	virtual void ChangeClip();
	FWeaponUIData GetUIData() const;
	FAmmoData GetAmmoData() const;
	bool TryToAddAmmo(int32 Clips);
	bool IsAmmoEmpty() const;

protected:
	virtual void BeginPlay() override;
	// Shoot
	virtual void MakeShot();
	virtual bool GetTraceData(FVector& TraceStart, FVector& TraceEnd);
	virtual void MakeHit(FHitResult& HitResult, const FVector& TraceStart, const FVector& TraceEnd) const;
	virtual void MakeDamage(const FHitResult& HitResult);
	APlayerController* GetPlayerController() const;
	AController* GetController() const;
	FVector GetMuzzleSocketLocation() const;
	bool GetPlayerViewPoint(FVector& ViewLocation, FRotator& ViewRotation) const;
	// Ammo
	virtual void DecreaseAmmo();
	bool IsClipEmpty() const;
	bool IsAmmoFull() const;
	// VFX
	UNiagaraComponent* SpawnMuzzleFX() const;

private:
	void LogAmmo() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName SocketName = "MuzzleSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (Units = "cm"))
	float TraceMaxDistance = 5000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Ammo")
	FAmmoData DefaultAmmo{15, 10, false};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Ammo")
	FWeaponUIData UIData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Trace FX")
	UNiagaraSystem* MuzzleFX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Sound")
	USoundCue* FireSound;

private:
	// Shoot
	FTimerHandle ShotTimer;
	bool bCanAim = true;
	// Ammo
	FAmmoData CurrentAmmo;
};
