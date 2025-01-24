// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Actor/Equipment/P12EquipableItem.h"
#include "Player/P12BaseCharacter.h"
#include "P12RangeWeaponItem.generated.h"

class UP12WeaponBarrelComponent;

UENUM()
enum class EP12FireMode : uint8
{
	Single,
	FullAuto
};

USTRUCT(BlueprintType)
struct FP12RecoilParams
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = -2.f, UIMin = -2.f, ClampMax = 2.f, UIMax = 2.f))
	float Yaw = -0.1f;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = -2.f, UIMin = -2.f, ClampMax = 2.f, UIMax = 2.f))
	float Pitch = 0.2f;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 0.f, UIMin = 0.f))
	float TimeInterval = 0.05f;
	UPROPERTY(EditDefaultsOnly, meta = (ClampMin = 1.f, UIMin = 1.f))
	float RollbackSpeed = 1200.f; /* In shots per minute. */
};

struct FP12AccumulatedRecoil
{
	void Reset()
	{
		Pitch = 0.f;
		Yaw = 0.f;
		Shots = 0;
		RollbackTime = 0.f;
	}

	float Pitch = 0.f;
	float Yaw = 0.f;
	int32 Shots = 0;
	float RollbackTime = 0.f;
};

UCLASS(Blueprintable)
class PROJECT12_API AP12RangeWeaponItem : public AP12EquipableItem, public IP12SaveSubsystemInterface
{
	GENERATED_BODY()

public:
	AP12RangeWeaponItem();
	virtual void Tick(const float DeltaTime) override;
	FORCEINLINE float GetAimSpeed() const { return AimSpeed; }
	FORCEINLINE UP12WeaponBarrelComponent* GetBarrelComponent() const { return WeaponBarrel; }
	FORCEINLINE float GetAimingFOV() const { return AimingFOV; };
	FTransform GetGripTransform() const;
	void FireInput(const bool bStart);
	void AimInput(const bool bStart);

	FORCEINLINE int32 GetAmmo() const { return Ammo; }
	FORCEINLINE int32 GetMaxAmmo() const { return MaxAmmo; }
	FORCEINLINE EP12AmmunitionType GetAmmoType() const { return AmmoType; }
	void SetAmmo(const int32 NewAmmo);
	bool GetCanShoot();

	void StartReloading();
	void OnFullReloadedHandle(USkeletalMeshComponent* SkeletalMeshComponent);
	void OnOneReloadedHandle(USkeletalMeshComponent* SkeletalMeshComponent, int32 NumberOfAmmo);
	void Reload(const bool bByOne = false, const int32 NumberOfAmmo = 1);
	void FinishReload(const bool bJumpToEnd = true);

	virtual void AttachItem(const FName AttachSocketName) override;
	virtual void OnLevelDeserialized_Implementation() override;

protected:
	virtual void BeginPlay() override;
	virtual void InitAnimNotify() override;

private:
	float PlayAnimMontage(UAnimMontage* AnimMontage, const float InPlayRate = 1, const FName StartSectionName = NAME_None) const;
	void StopAnimMontage(UAnimMontage* AnimMontage);
	float GetShotTimeInterval();
	void MakeShot();
	void RefreshAmmoCount() const;
	UP12EquipmentComponent* GetEquipment() const;
	float GetRecoilTimeInterval();
	void ProcessRecoil(const float DeltaTime);
	void ProcessRecoilRollback(const float DeltaTime);
	void StopRecoilRollback();

protected:
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	USkeletalMeshComponent* WeaponMesh;
	UPROPERTY(VisibleAnywhere, Category = "C++ | Component")
	UP12WeaponBarrelComponent* WeaponBarrel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Alignment")
	FName MuzzleSocketName = "Muzzle_Socket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Alignment")
	FVector WeaponMeshOffset = FVector(-1.f, -2.f, -3.f);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Alignment")
	FRotator WeaponMeshOrientation = FRotator(20.f, 0.f, 0.f);

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Animation | Weapon")
	UAnimMontage* WeaponFireMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Animation | Weapon")
	UAnimMontage* WeaponReloadMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Animation | Character")
	UAnimMontage* CharacterFireMontage;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Animation | Character")
	UAnimMontage* CharacterReloadMontage;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire")
	EP12FireMode FireMode = EP12FireMode::Single;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire", meta = (ClampMin = 1.f, UIMin = 1.f, ToolTip = "Shots per minute."))
	float ShotRate = 600.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire", meta = (Units = "deg"))
	float BulletSpread = 5.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire", meta = (Units = "deg"))
	float AimBulletSpread = 2.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire")
	float AimSpeed = 200.f;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire")
	EP12AmmunitionType AmmoType = EP12AmmunitionType::None;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire")
	bool bAutoReload = true;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire")
	int32 MaxAmmo = 30;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Fire")
	float AimingFOV = 90.f;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Recoil")
	FP12RecoilParams RecoilParams;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Recoil")
	TSubclassOf<UCameraShakeBase> ShotCameraShakeClass;

private:
	FTimerHandle ShotTimer;
	float CurrentBulletSpread = 0.f;
	bool bAiming = false;
	UPROPERTY(SaveGame)
	int32 Ammo = 0;
	bool bReloading = false;
	float LastShotTime = 0.f;

	FTimerHandle RecoilTimer;
	FTimerHandle RecoilRollbackTimer;
	FP12AccumulatedRecoil AccumulatedRecoil;
};
