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

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

#pragma region Weapon

public:
	virtual void StartFire();
	virtual void NextWeapon();

	void StopFire();
	void Aiming();
	void Reload();
	bool GetWeaponUIData(FWeaponUIData& UIData) const;
	bool GetWeaponAmmoData(FAmmoData& AmmoData) const;
	bool TryToAddAmmo(TSubclassOf<ASTU_BaseWeapon> WeaponType, int32 Clips);

protected:
	bool CanFire() const;
	bool CanEquip() const;
	void EquipWeapon(int32 Index);

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

private:
	void SpawnWeapons();
	void AttachWeaponToSocket(ASTU_BaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void OnClipEmpty(ASTU_BaseWeapon* EmptyWeapon);
	void ChangeClip();
	bool CanReload() const;

#pragma endregion // Weapon

#pragma region Animation

public:
	UFUNCTION(BlueprintCallable, Category = "Kinder | Weapon")
	bool IsWeaponChanging() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | Weapon")
	bool IsWeaponReloading() const;

protected:
	void PlayAnimMontage(UAnimMontage* Animation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	UAnimMontage* EquipAnimation = nullptr;

	bool bWeaponChanging  = false;
	bool bWeaponReloading = false;

private:
	void InitAnimations();

	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnChangedFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimation = nullptr;

#pragma endregion // Animation
};
