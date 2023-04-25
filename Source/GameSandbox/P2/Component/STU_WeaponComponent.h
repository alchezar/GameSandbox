// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameSandbox/P2/STU_CoreTypes.h"
#include "STU_WeaponComponent.generated.h"

class ASTU_BaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API USTU_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTU_WeaponComponent();
	// Weapon
	virtual void StartFire();
	virtual void NextWeapon();
	void StopFire();
	void Aiming();
	void ToggleAim(const bool bAim);
	void Reload();
	bool GetWeaponUIData(FWeaponUIData& UIData) const;
	bool GetWeaponAmmoData(FAmmoData& AmmoData) const;
	bool TryToAddAmmo(TSubclassOf<ASTU_BaseWeapon> WeaponType, int32 Clips);
	bool NeedAmmo(TSubclassOf<ASTU_BaseWeapon> WeaponType);
	// Animation
	bool GetIsWeaponChanging() const;
	bool GetIsWeaponReloading() const;
	void SetIsWeaponChanging(const bool bChanging);
	void SetIsWeaponReloading(const bool bReloading);

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	// Weapon
	bool CanFire() const;
	bool CanEquip() const;
	void EquipWeapon(int32 Index);
	// Animation
	void PlayAnimMontage(UAnimMontage* Animation);

private:
	// Weapon
	void SpawnWeapons();
	void AttachWeaponToSocket(ASTU_BaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void OnClipEmpty(ASTU_BaseWeapon* EmptyWeapon);
	void ChangeClip();
	bool CanReload() const;
	// Animation
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnChangedFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);
	template <typename T>
	static T* FindNotifyByClass(UAnimSequenceBase* Animation);

protected:
	// Weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	TArray<FWeaponData> WeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName HandSocketName = "HandWeaponSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName BackSocketName = "ArmorySocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (ClampMin = 0, ClampMax = 2))
	int32 CurrentWeaponIndex = 0;
	UPROPERTY()
	ASTU_BaseWeapon* CurrentWeapon = nullptr;
	UPROPERTY()
	TArray<ASTU_BaseWeapon*> Weapons;
	// Animation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	UAnimMontage* EquipAnimation = nullptr;

private:
	// Animation
	UPROPERTY()
	UAnimMontage* CurrentReloadAnimation = nullptr;
	bool bWeaponChanging                 = false;
	bool bWeaponReloading                = false;
};
