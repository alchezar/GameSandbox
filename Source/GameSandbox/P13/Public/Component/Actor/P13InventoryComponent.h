// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P13/Public/Library/P13Types.h"
#include "P13InventoryComponent.generated.h"

class UP13GameInstance;
DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnSwitchWeaponSignature, FName /*WeaponID*/, const FP13WeaponDynamicInfo* /*Info*/, const int32 /*CurrentIndex*/)
DECLARE_MULTICAST_DELEGATE_ThreeParams(FP13OnAmmoChangedSignature, EP13WeaponType /*AmmoType*/, int32 /*NewWeaponAmmoCount*/, int32 /*NewInventoryAmmoCount*/)
DECLARE_MULTICAST_DELEGATE(FP13OnInventoryUpdatedSingature);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP13InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	UP13InventoryComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FORCEINLINE int32 GetWeaponSlotsCount() const { return WeaponSlots.Num(); }
	FORCEINLINE int32 GetAmmoSlotsCount() const { return AmmoSlots.Num(); }
	FORCEINLINE FP13WeaponSlot GetWeaponSlot(const int32 Index) const { return WeaponSlots[Index]; }
	FORCEINLINE FP13AmmoSlot GetAmmoSlot(const int32 Index) const { return AmmoSlots[Index]; }
	int32 GetWeaponSlotIndex(const FName WeaponID) const;
	FName GetWeaponIdBySlotIndex(const int32 Index = 0) const;
	FP13WeaponDynamicInfo GetWeaponDynamicInfo(const int32 Index);
	
	void SetWeaponInfo(const int32 WeaponIndex, const FP13WeaponDynamicInfo NewInfo, const bool bIncrease = false);
	bool TrySwitchWeaponToIndex(const int32 NewIndex, int32 OldIndex, FP13WeaponDynamicInfo OldInfo);
	int32 FindMaxAvailableRound(const int32 NewOldRoundNum, const int32 WeaponIndex, const int32 MaxRound);

private:
	bool TryUpdateSlotsFromData(const UP13GameInstance* GameInstance);
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
public:
	FP13OnSwitchWeaponSignature OnSwitchWeapon;
	FP13OnAmmoChangedSignature OnAmmoChanged;
	FP13OnInventoryUpdatedSingature OnInventoryUpdated;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	TArray<FP13WeaponSlot> WeaponSlots;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Weapon")
	TArray<FP13AmmoSlot> AmmoSlots;

private:
	int32 AmmoLeft = 0;
};
