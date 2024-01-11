// Copyright © 2024, IKinder

#include "P13/Public/Component/Actor/P13InventoryComponent.h"

#include "P13/Public/Actor/P13PickupWeapon.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/Weapon/P13Weapon.h"

UP13InventoryComponent::UP13InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP13InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	/* Find init weapon slots and find init weapons. */
	GameInstanceCached = Cast<UP13GameInstance>(GetWorld()->GetGameInstance());
	check(GameInstanceCached.IsValid())

	TryUpdateSlotsFromData();
}

void UP13InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

bool UP13InventoryComponent::TryTakeWeaponToInventory(const FP13WeaponSlot& NewWeaponSlot)
{
	/* Check if we can take weapon.
	 * If weapon is already exist in the inventory - we can't take it. */
	if (WeaponSlots.FindByPredicate([&](const FP13WeaponSlot& Slot) { return Slot.WeaponID == NewWeaponSlot.WeaponID; }))
	{
		return false;
	}

	/* Take weapon to the inventory. */
	WeaponSlots.Add(NewWeaponSlot);
	
	OnNewWeaponTaken.Broadcast(WeaponSlots.Num() - 1, NewWeaponSlot);
	OnSwitchWeapon.Broadcast(WeaponSlots[CurrentWeaponIndex], CurrentWeaponIndex);
	return true;
}

bool UP13InventoryComponent::TryTakeAmmoToInventory(const FP13AmmoSlot& NewAmmoSlot)
{
	/* Try to find a similar slot in the inventory. */
	FP13AmmoSlot* CorrectSlot = AmmoSlots.FindByPredicate([&](const FP13AmmoSlot& Slot)
	{
		return Slot.WeaponType == NewAmmoSlot.WeaponType;
	});

	/* Add a new slot if it didn't exist before. */
	if (!CorrectSlot)
	{
		AmmoSlots.Add(NewAmmoSlot);
		OnNewAmmoTaken.Broadcast(NewAmmoSlot);
		return true;
	}

	/* If the ammo is full - we can't add more. */
	if (CorrectSlot->Count >= CorrectSlot->MaxCount)
	{
		return false;
	}
	
	/* Take ammo to the inventory. */
	CorrectSlot->Count = FMath::Min(CorrectSlot->Count + NewAmmoSlot.Count, CorrectSlot->MaxCount);

	OnAmmoChanged.Broadcast(CorrectSlot->WeaponType, -1, CorrectSlot->Count);
	return true;
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

void UP13InventoryComponent::SetWeaponInfo(const FP13WeaponDynamicInfo NewInfo, const bool bIncrease)
{
	/* Check if there is available weapon at this index. */
	if ((WeaponSlots.Num() - 1 < CurrentWeaponIndex) || WeaponSlots[CurrentWeaponIndex].WeaponID.IsNone())
	{
		return;
	}

	/* Find correct ammo slot. */
	FP13AmmoSlot* AmmoSlot = AmmoSlots.FindByPredicate([&](const FP13AmmoSlot& Slot)
	{
		return Slot.WeaponType == WeaponSlots[CurrentWeaponIndex].AmmoType;
	});
	check(AmmoSlot)

	/* Set new dynamic info into weapon slot. */
	WeaponSlots[CurrentWeaponIndex].DynamicInfo = NewInfo;
	int32 RequiredAmmo = NewInfo.Round;

	/* If we reloading, check max available ammo amount for weapon. */
	if (bIncrease)
	{
		/* Clam required by all available ammo (inventory + magazine). */
		RequiredAmmo = FMath::Clamp(RequiredAmmo, 0, AmmoSlot->Count + AmmoLeft);
		/* Remove only the lack of ammo from the inventory. */
		AmmoSlot->Count -= (RequiredAmmo - AmmoLeft);
	}

	OnAmmoChanged.Broadcast(WeaponSlots[CurrentWeaponIndex].AmmoType, RequiredAmmo, AmmoSlot->Count);
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

		CurrentWeaponIndex = Index;
		OnSwitchWeapon.Broadcast(WeaponSlots[Index], Index);
		return true;
	}
	/* Weapon switch wasn't successful. */
	// ...
	return false;
}

int32 UP13InventoryComponent::FindMaxAvailableRound(const int32 OldRoundNum, const int32 MaxRound)
{
	/* We cache the number of rounds before reload. */
	AmmoLeft = OldRoundNum;

	/* Find max round amount for this reload. */
	const int32 MaxClamp = AmmoSlots[CurrentWeaponIndex].Count + OldRoundNum;
	const int32 MagazineSize = FMath::Clamp(MaxRound, 0, MaxClamp);
	return MagazineSize;
}

void UP13InventoryComponent::DropCurrentWeapon(const AP13Weapon* CurrentWeapon, const bool bTakeNext)
{
	check(GameInstanceCached)
	check(CurrentWeapon)

	/* Avoid dropping last weapon. */
if (WeaponSlots.Num() <= 1)
	{
		return;
	}

	/* Find and update info about dropped item. */
	FP13WeaponDrop* DropWeaponInfo = GameInstanceCached->GetWeaponDropByID(WeaponSlots[CurrentWeaponIndex].WeaponID);
	check(DropWeaponInfo)
	DropWeaponInfo->WeaponInfo = WeaponSlots[CurrentWeaponIndex];
	
	/* Spawn dropped weapon. */
	const FVector SpawnLocation = CurrentWeapon->GetActorLocation() + CurrentWeapon->GetMesh()->GetLocalBounds().Origin; 
	const FTransform SpawnTransform = {CurrentWeapon->GetActorRotation(), SpawnLocation};
	
	AP13PickupWeapon* DroppedWeapon = GetWorld()->SpawnActorDeferred<AP13PickupWeapon>(AP13PickupWeapon::StaticClass(), SpawnTransform);
	if (!DroppedWeapon)
	{
		return;
	}	
	DroppedWeapon->InitDrop(DropWeaponInfo);
	DroppedWeapon->FinishSpawning(SpawnTransform);

	if (!bTakeNext)
	{
		return;
	}
	
	const int32 RemoveIndex = CurrentWeaponIndex;
	if (!TrySwitchWeaponToIndex(CurrentWeaponIndex + 1, CurrentWeaponIndex, WeaponSlots[CurrentWeaponIndex].DynamicInfo))
	{
		return;
	}
	WeaponSlots.RemoveAt(RemoveIndex);
	CurrentWeaponIndex = RemoveIndex < CurrentWeaponIndex ? CurrentWeaponIndex - 1 : CurrentWeaponIndex;
	
	OnInventoryUpdated.Broadcast();
	OnSwitchWeapon.Broadcast(WeaponSlots[CurrentWeaponIndex], CurrentWeaponIndex);
}

void UP13InventoryComponent::ClearWeaponSlots()
{
	WeaponSlots.Empty();
}

void UP13InventoryComponent::SortAmmoSlots()
{
	for (int32 WeaponIndex = 0; WeaponIndex < WeaponSlots.Num(); ++WeaponIndex)
	{
		if (!AmmoSlots.IsValidIndex(WeaponIndex))
		{
			return;
		}

		if (WeaponSlots[WeaponIndex].AmmoType == AmmoSlots[WeaponIndex].WeaponType)
		{
			continue;
		}

		for (int32 AmmoIndex = WeaponIndex; AmmoIndex < AmmoSlots.Num(); ++AmmoIndex)
		{
			if (WeaponSlots[WeaponIndex].AmmoType != AmmoSlots[AmmoIndex].WeaponType)
			{
				continue;
			}
			AmmoSlots.Swap(WeaponIndex, AmmoIndex);
		}
	}
}

bool UP13InventoryComponent::TryUpdateSlotsFromData()
{
	/* Find init weapon slots and find init weapons. */
	for (int32 Index = 0; Index < WeaponSlots.Num(); ++Index)
	{
		FName WeaponID = WeaponSlots[Index].WeaponID;
		if (WeaponID.IsNone())
		{
			WeaponSlots.RemoveAt(Index);
			--Index;
			continue;
		}
		const FP13WeaponInfo* WeaponInfo = GameInstanceCached->GetWeaponInfoByID(WeaponID);
		if (!WeaponInfo)
		{
			WeaponSlots.RemoveAt(Index);
			--Index;
			continue;
		}

		/* Update info in weapon slot at the game beginning. */
		WeaponSlots[Index].AmmoType = WeaponInfo->AmmoType;
		WeaponSlots[Index].MaxRound = WeaponInfo->MaxRound;
		WeaponSlots[Index].DynamicInfo = {WeaponInfo->MaxRound};
	}

	if (!WeaponSlots.IsValidIndex(0) || WeaponSlots[0].WeaponID.IsNone())
	{
		return false;
	}

	SortAmmoSlots();
	OnInventoryUpdated.Broadcast();
	OnSwitchWeapon.Broadcast(WeaponSlots[CurrentWeaponIndex], CurrentWeaponIndex);
	return true;
}
