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
	void StartFire();
	void StopFire();
	void Aiming();
	void NextWeapon();
	void Reload();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	TArray<FWeaponData> WeaponData;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName HandSocketName = "HandWeaponSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName BackSocketName = "ArmorySocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (ClampMin = 0, ClampMax = 1))
	int32 CurrentWeaponIndex = 0;

private:
	void SpawnWeapons();
	void AttachWeaponToSocket(ASTU_BaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 Index);

	bool CanFire() const;
	bool CanReload() const;

	void OnEmptyClip();
	void ChangeClip();

	UPROPERTY()
	ASTU_BaseWeapon* CurrentWeapon = nullptr;
	UPROPERTY()
	TArray<ASTU_BaseWeapon*> Weapons;

#pragma endregion // Weapon

#pragma region Animation

public:
	UFUNCTION(BlueprintCallable, Category = "Kinder | Weapon")
	bool IsWeaponChanging() const;
	UFUNCTION(BlueprintCallable, Category = "Kinder | Weapon")
	bool IsWeaponReloading() const;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	UAnimMontage* EquipAnimation = nullptr;

private:
	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnChangedFinished(USkeletalMeshComponent* MeshComp);
	void OnReloadFinished(USkeletalMeshComponent* MeshComp);

	UPROPERTY()
	UAnimMontage* CurrentReloadAnimation = nullptr;

	bool bWeaponChanging = false;
	bool bWeaponReloading = false;

#pragma endregion // Animation
};
