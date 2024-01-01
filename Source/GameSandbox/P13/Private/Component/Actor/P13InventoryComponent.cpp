// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13InventoryComponent.h"

#include "P13/Public/Game/P13GameInstance.h"

UP13InventoryComponent::UP13InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP13InventoryComponent::BeginPlay()
{
	Super::BeginPlay();
	
	/* Find init weapon slots and find init weapons. */
	const UP13GameInstance* GameInstance = Cast<UP13GameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
	{
		return;
	}

	if (!TryUpdateSlotsFromData(GameInstance))
	{
		return;
	}
	
	// FP13WeaponDynamicInfo DataInfo = &WeaponSlots[0].DynamicInfo;
	const FP13WeaponDynamicInfo DataInfo = {GameInstance->GetWeaponInfoByID(WeaponSlots[0].WeaponID)->MaxRound};
	OnSwitchWeapon.Broadcast(WeaponSlots[0].WeaponID, &DataInfo, 0);
}

void UP13InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32 UP13InventoryComponent::GetWeaponSlotIndex(const FName WeaponID) const
{
	for (int32 Index = 0; Index < WeaponSlots.Num(); ++Index)
	{
		if (WeaponSlots[Index].WeaponID != WeaponID)
		{
			continue;
		}
		return Index;
	}
	return -1;
}

FName UP13InventoryComponent::GetWeaponIdBySlotIndex(const int32 Index) const
{
	return WeaponSlots[Index].WeaponID;
}

FP13WeaponDynamicInfo UP13InventoryComponent::GetWeaponDynamicInfo(const int32 Index)
{
	return WeaponSlots[Index].DynamicInfo;
}

void UP13InventoryComponent::SetWeaponInfo(const int32 WeaponIndex, const FP13WeaponDynamicInfo NewInfo)
{
	/* Check if there is a weapon at this index. */
	if ((WeaponSlots.Num() - 1 < WeaponIndex) || WeaponSlots[WeaponIndex].WeaponID.IsNone())
	{
		return;
	}

	WeaponSlots[WeaponIndex].DynamicInfo = NewInfo;
	AmmoSlots[WeaponIndex].Count = NewInfo.Round;

	OnAmmoChanged.Broadcast(AmmoSlots[WeaponIndex].WeaponType, AmmoSlots[WeaponIndex].Count);
}

bool UP13InventoryComponent::TrySwitchWeaponToIndex(const int32 NewIndex, int32 OldIndex, FP13WeaponDynamicInfo OldInfo)
{
	const int32 CorrectIndex = NewIndex >= 0 ? NewIndex % WeaponSlots.Num() : WeaponSlots.Num() + NewIndex;

	for (int32 Index = 0; Index < WeaponSlots.Num(); ++Index)
	{
		if ((Index != CorrectIndex) || WeaponSlots[Index].WeaponID.IsNone())
		{
			continue;
		}
		
		OnSwitchWeapon.Broadcast(WeaponSlots[Index].WeaponID, &WeaponSlots[Index].DynamicInfo, Index);
		return true;
	}
	/* Weapon switch wasn't successful. */
	
	return false;
}

bool UP13InventoryComponent::TryUpdateSlotsFromData(const UP13GameInstance* GameInstance)
{
	/* Find init weapon slots and find init weapons. */
	for (int32 Index = 0; Index < WeaponSlots.Num(); ++Index)
	{
		FName WeaponID = WeaponSlots[Index].WeaponID;
		if (WeaponID.IsNone())
		{
			continue;
		}
		const FP13WeaponInfo* WeaponInfo = GameInstance->GetWeaponInfoByID(WeaponID);
		if (!WeaponInfo)
		{
			WeaponSlots.RemoveAt(Index);
			--Index;
			continue;
		}

		AmmoSlots[Index].MaxCount = WeaponInfo->MaxRound;
		SetWeaponInfo(Index, {WeaponInfo->MaxRound});
	}
	
	if (!WeaponSlots.IsValidIndex(0) || WeaponSlots[0].WeaponID.IsNone())
	{
		return false;
	}

	OnInventoryUpdated.Broadcast();
	return true;
}
