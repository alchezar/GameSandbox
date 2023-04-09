// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "STU_WeaponComponent.generated.h"

class ASTU_BaseWeapon;
class UAnimMontage;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API USTU_WeaponComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	USTU_WeaponComponent();

	void StartFire();
	void StopFire();
	void NextWeapon();

protected:
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	TArray<TSubclassOf<ASTU_BaseWeapon>> WeaponClasses;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName HandSocketName = "HandWeaponSocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	FName BackSocketName = "ArmorySocket";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon", meta = (ClampMin = 0, ClampMax = 1))
	int32 CurrentWeaponIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Weapon")
	UAnimMontage* EquipAnimation = nullptr;

private:
	void SpawnWeapons();
	void AttachWeaponToSocket(ASTU_BaseWeapon* Weapon, USceneComponent* SceneComponent, const FName& SocketName);
	void EquipWeapon(int32 Index);
	void PlayAnimMontage(UAnimMontage* Animation);
	void InitAnimations();
	void OnEquipFinished(USkeletalMeshComponent* MeshComp);
	void OnChangedFinished(USkeletalMeshComponent* MeshComp);

	UPROPERTY()
	ASTU_BaseWeapon* CurrentWeapon = nullptr;
	UPROPERTY()
	TArray<ASTU_BaseWeapon*> Weapons;
};
