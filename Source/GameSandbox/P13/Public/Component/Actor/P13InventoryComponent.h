// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P13/Public/Intearface/P13PickupInterface.h"
#include "P13/Public/Library/P13Types.h"
#include "P13InventoryComponent.generated.h"

class UP13GameInstance;
class AP13Weapon;

DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnSwitchWeaponStartSignature, const FP13WeaponSlot& /*NewWeaponSLot*/, const int32 /*CurrentIndex*/)
DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnSwitchWeaponFinishSignature, const int32 /*NewWeaponIndex*/, AP13Weapon* /*NewWeapon*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnAmmoChangedSignature, EP13AmmoType /*AmmoType*/, int32 /*NewWeaponAmmoCount*/, int32 /*NewInventoryAmmoCount*/)
DECLARE_MULTICAST_DELEGATE(FP13OnInventoryUpdatedSingature);
DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnNewWeaponTakenSignature, const int32 /*NewWeaponIndex*/, const FP13WeaponSlot& /*NewWeaponSLot*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FP13OnNewAmmoTakenSignature, const FP13AmmoSlot& /*NewAmmoSlot*/)

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP13InventoryComponent : public UActorComponent, public IP13PickupInterface
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	UP13InventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ----------------------------- Interface ----------------------------- */
public:
	virtual bool TryTakeWeaponToInventory(const FP13WeaponSlot& NewWeaponSlot) override;
	virtual bool TryTakeAmmoToInventory(const FP13AmmoSlot& NewAmmoSlot) override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE int32 GetWeaponSlotsCount() const { return WeaponSlots.Num(); }
	FORCEINLINE int32 GetAmmoSlotsCount() const { return AmmoSlots.Num(); }
	FORCEINLINE int32 GetCurrentWeaponIndex() const { return CurrentWeaponIndex; }
	FORCEINLINE FP13WeaponSlot GetWeaponSlot(const int32 Index) const { return WeaponSlots[Index]; }
	FORCEINLINE FP13AmmoSlot GetAmmoSlot(const int32 Index) const { return AmmoSlots[Index]; }
	int32 GetWeaponSlotIndex(const FName WeaponID) const;
	FName GetWeaponIdBySlotIndex(const int32 Index = 0) const;
	FP13WeaponDynamicInfo GetWeaponDynamicInfo(const int32 Index);
	void SetWeaponInfo(const FP13WeaponDynamicInfo NewInfo, const bool bIncrease = false);
	bool TrySwitchWeaponToIndex(const int32 NewIndex, int32 OldIndex, FP13WeaponDynamicInfo OldInfo);
	int32 FindMaxAvailableRound(const int32 OldRoundNum, const int32 MaxRound);
	void DropCurrentWeapon(const AP13Weapon* CurrentWeapon, const bool bTakeNext = true);
	void ClearWeaponSlots();
	void SortAmmoSlots();

private:
	bool TryUpdateSlotsFromData();

	/* ----------------------------- Variables ----------------------------- */
public:
	FP13OnSwitchWeaponStartSignature OnSwitchWeaponStart;
	FP13OnSwitchWeaponFinishSignature OnSwitchWeaponFinish;
	FP13OnAmmoChangedSignature OnAmmoChanged;
	FP13OnInventoryUpdatedSingature OnInventoryUpdated;
	FP13OnNewWeaponTakenSignature OnNewWeaponTaken;
	FP13OnNewAmmoTakenSignature OnNewAmmoTaken;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	TArray<FP13WeaponSlot> WeaponSlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	TArray<FP13AmmoSlot> AmmoSlots;

private:
	int32 AmmoLeft = 0;
	TSoftObjectPtr<UP13GameInstance> GameInstanceCached;
	int32 CurrentWeaponIndex = 0;
};
