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
		WeaponSlots[Index].DynamicInfo.Round = FP13WeaponInfo{}.MaxRound;
	}
	
	if (!WeaponSlots.IsValidIndex(0) || WeaponSlots[0].WeaponID.IsNone())
	{
		return;
	}
	OnSwitchWeapon.Broadcast(WeaponSlots[0].WeaponID, &WeaponSlots[0].DynamicInfo);
}

void UP13InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

int32 UP13InventoryComponent::GetWeaponSlotIndex(const FName WeaponID) const
{
	// return WeaponSlots.FindByPredicate([&](const FP13WeaponSlot& Slot){ return Slot.WeaponID == WeaponID; })->IndexSlot;

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
	// return WeaponSlots.FindByPredicate([&](const FP13WeaponSlot& Slot){ return Slot.IndexSlot == Index; })->DynamicInfo;

	return WeaponSlots[Index].DynamicInfo;
}

void UP13InventoryComponent::SetWeaponInfo(const int32 WeaponIndex, const FP13WeaponDynamicInfo NewInfo)
{
	if ((WeaponSlots.Num() - 1 < WeaponIndex) || WeaponSlots[WeaponIndex].WeaponID.IsNone())
	{
		return;
	}

	WeaponSlots[WeaponIndex].DynamicInfo = NewInfo;
}

bool UP13InventoryComponent::TrySwitchWeaponToIndex(const int32 NewIndex, int32 OldIndex, FP13WeaponDynamicInfo OldInfo)
{
	const int32 CorrectIndex = NewIndex >= 0 ? NewIndex % WeaponSlots.Num() : WeaponSlots.Num() + NewIndex;

	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("%d"), CorrectIndex));

	for (int32 Index = 0; Index < WeaponSlots.Num(); ++Index)
	{
		if ((Index != CorrectIndex) || WeaponSlots[Index].WeaponID.IsNone())
		{
			continue;
		}
		
		OnSwitchWeapon.Broadcast(WeaponSlots[Index].WeaponID, &WeaponSlots[Index].DynamicInfo);
		return true;
	}
	/* Weapon switch wasn't successful. */
	
	return false;
}
