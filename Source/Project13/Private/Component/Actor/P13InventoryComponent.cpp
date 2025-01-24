// Copyright © 2024, IKinder

#include "Component/Actor/P13InventoryComponent.h"

#include "Actor/P13PickupActor.h"
#include "Game/P13GameInstance.h"
#include "Game/P13PlayerState.h"
#include "Net/UnrealNetwork.h"
#include "Weapon/P13Weapon.h"

UP13InventoryComponent::UP13InventoryComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UP13InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	/* We won`t be update our slots here at begin play, because after respawn at this point
	 * Pawn don`t have neither Controller nor PlayerState. Just wait for Pawn's OnPossess event. */
	CacheGameInstance();
}

void UP13InventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP13InventoryComponent::PickupAmmoToInventory(const FP13AmmoSlot& NewAmmoSlot, AP13PickingUpBase* PickupActor)
{
	Server_TakingAmmoToInventoryAttempt(NewAmmoSlot, PickupActor);
}

void UP13InventoryComponent::PickupWeaponToInventory(const FP13AmmoSlot& NewAmmoSlot, const FP13WeaponSlot& NewWeaponSlot, AP13PickingUpBase* PickupActor)
{
	Server_TakingWeaponToInventoryAttempt(NewAmmoSlot, NewWeaponSlot, PickupActor);
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

	Multicast_OnAmmoChangedBroadcast(WeaponSlots[CurrentWeaponIndex].AmmoType, RequiredAmmo, AmmoSlot->Count);
}

bool UP13InventoryComponent::TrySwitchWeaponToIndex(const int32 NewIndex, const int32 OldIndex, FP13WeaponDynamicInfo OldInfo, const bool bNext)
{
	const int32 CorrectIndex = NewIndex >= 0 ? NewIndex % WeaponSlots.Num() : WeaponSlots.Num() + NewIndex;

	/* Means we did a full recursion cycle. */
	if (CorrectIndex == OldIndex)
	{
		return false;
	}

	if (!WeaponSlots.IsValidIndex(CorrectIndex) || WeaponSlots[CorrectIndex].WeaponID.IsNone())
	{
		return false;
	}

	/* Don't switch to weapon which has no ammo. */
	if (WeaponSlots[CorrectIndex].DynamicInfo.Round + AmmoSlots[CorrectIndex].Count == 0)
	{
		const int32 NextDirection = bNext ? 1 : -1;
		return TrySwitchWeaponToIndex(CorrectIndex + NextDirection, OldIndex, OldInfo);
	}

	CurrentWeaponIndex = CorrectIndex;
	Multicast_OnCurrentWeaponUpdatedBroadcast(WeaponSlots[CorrectIndex], CorrectIndex);
	return true;
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

bool UP13InventoryComponent::TryDropCurrentWeapon(const AP13Weapon* CurrentWeapon, bool bTakeNext)
{
	check(GameInstanceCached)
	if (!CurrentWeapon)
	{
		return false;
	}

	/* If we dropping the last weapon, don't try to take next one. */
	if (WeaponSlots.Num() == 1)
	{
		bTakeNext = false;
	}

	/* Find and update info about dropped item. */
	FP13WeaponDrop* DropWeaponInfo = GameInstanceCached->GetWeaponDropByID(WeaponSlots[CurrentWeaponIndex].WeaponID);
	check(DropWeaponInfo)
	DropWeaponInfo->WeaponInfo = WeaponSlots[CurrentWeaponIndex];

	/* Spawn dropped weapon. */
	const FVector SpawnLocation = CurrentWeapon->GetActorLocation() + CurrentWeapon->GetMesh()->GetLocalBounds().Origin;
	const FTransform SpawnTransform = {CurrentWeapon->GetActorRotation(), SpawnLocation};

	Server_SpawnDroppedWeapon(SpawnTransform, *DropWeaponInfo);

	if (!bTakeNext)
	{
		WeaponSlots.RemoveAt(CurrentWeaponIndex);
		Multicast_OnInventoryUpdatedBroadcast();
		return false;
	}

	const int32 RemoveIndex = CurrentWeaponIndex;
	if (!TrySwitchWeaponToIndex(CurrentWeaponIndex + 1, CurrentWeaponIndex, WeaponSlots[CurrentWeaponIndex].DynamicInfo))
	{
		return false;
	}
	WeaponSlots.RemoveAt(RemoveIndex);
	CurrentWeaponIndex = RemoveIndex < CurrentWeaponIndex ? CurrentWeaponIndex - 1 : CurrentWeaponIndex;

	Multicast_OnInventoryUpdatedBroadcast();
	Multicast_OnCurrentWeaponUpdatedBroadcast(WeaponSlots[CurrentWeaponIndex], CurrentWeaponIndex);
	return true;
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

void UP13InventoryComponent::RefreshSlots()
{
	if (!TryLoadSlotsFromPlayerState())
	{
		TryUpdateSlotsFromData();
	}
}

void UP13InventoryComponent::CacheGameInstance()
{
	if (GameInstanceCached.IsValid())
	{
		return;
	}

	GameInstanceCached = Cast<UP13GameInstance>(GetWorld()->GetGameInstance());
	check(GameInstanceCached.IsValid())
}

bool UP13InventoryComponent::TryUpdateSlotsFromData()
{
	CacheGameInstance();

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
	Multicast_OnInventoryUpdatedBroadcast();
	Multicast_OnCurrentWeaponUpdatedBroadcast(WeaponSlots[0], 0);
	return true;
}

bool UP13InventoryComponent::TryLoadSlotsFromPlayerState()
{
	const APawn* PawnOwner = GetOwner<APawn>();
	if (!PawnOwner)
	{
		return false;
	}
	const AController* PawnController = PawnOwner->Controller;
	if (!PawnController)
	{
		return false;
	}
	AP13PlayerState* PlayerState = PawnController->GetPlayerState<AP13PlayerState>();
	if (!PlayerState)
	{
		return false;
	}
	if (!PlayerState->GetCanLoadPlayerInventory())
	{
		return false;
	}

	WeaponSlots = PlayerState->GetSavedWeaponSlots();
	AmmoSlots = PlayerState->GetSavedAmmoSlots();

	Multicast_OnInventoryUpdatedBroadcast();
	Multicast_OnCurrentWeaponUpdatedBroadcast(WeaponSlots[0], 0);

	return true;
}

void UP13InventoryComponent::TakingAmmoToInventoryAttempt(const FP13AmmoSlot& NewAmmoSlot, AP13PickingUpBase* PickupActor)
{
	if (PickupActor && TryTakeAmmoToInventory(NewAmmoSlot))
	{
		PickupActor->OnPickupSuccess();
	}
}

void UP13InventoryComponent::TakingWeaponToInventoryAttempt(const FP13AmmoSlot& NewAmmoSlot, const FP13WeaponSlot& NewWeaponSlot, AP13PickingUpBase* PickupActor)
{
	/* Try to add weapon to the inventory. */
	bool bTaken = TryTakeWeaponToInventory(NewWeaponSlot);

	/* When we successfully took the weapon, but there is no slot for its ammo in the inventory -
	 * try to add an empty one, by resetting count in ammo slot.
	 * Or if we already have this weapon in the inventory - simply add one magazine to its ammo slot. */
	FP13AmmoSlot UpdatedAmmoSlot = NewAmmoSlot;
	UpdatedAmmoSlot.Count = bTaken ? 0 : NewAmmoSlot.Count;
	bTaken |= TryTakeAmmoToInventory(UpdatedAmmoSlot);

	if (PickupActor && bTaken)
	{
		PickupActor->OnPickupSuccess();
	}
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

	/* If our current weapon hasn't ammo - switch to just taken. */
	if (WeaponSlots[CurrentWeaponIndex].DynamicInfo.Round == 0)
	{
		TrySwitchWeaponToIndex(CurrentWeaponIndex + 1, CurrentWeaponIndex, WeaponSlots[CurrentWeaponIndex].DynamicInfo);
	}

	Multicast_OnNewWeaponTakenBroadcast(WeaponSlots.Num() - 1, NewWeaponSlot);
	Multicast_OnCurrentWeaponUpdatedBroadcast(WeaponSlots[CurrentWeaponIndex], CurrentWeaponIndex);
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
		Multicast_OnNewAmmoTakenBroadcast(NewAmmoSlot);
		return true;
	}

	/* If the ammo is full - we can't add more. */
	if (CorrectSlot->Count >= CorrectSlot->MaxCount)
	{
		return false;
	}

	/* Take ammo to the inventory. */
	CorrectSlot->Count = FMath::Min(CorrectSlot->Count + NewAmmoSlot.Count, CorrectSlot->MaxCount);

	Multicast_OnAmmoChangedBroadcast(CorrectSlot->WeaponType, -1, CorrectSlot->Count);
	return true;
}

void UP13InventoryComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, WeaponSlots)
	DOREPLIFETIME(ThisClass, AmmoSlots)
}

void UP13InventoryComponent::Server_RefreshSlots_Implementation()
{
	RefreshSlots();
}

void UP13InventoryComponent::Server_SpawnDroppedWeapon_Implementation(const FTransform& SpawnTransform, const FP13WeaponDrop DropWeaponInfo)
{
	if (AP13PickingUpWeapon* DroppedWeapon = GetWorld()->SpawnActorDeferred<AP13PickingUpWeapon>(AP13PickingUpWeapon::StaticClass(), SpawnTransform))
	{
		DroppedWeapon->InitDrop(&DropWeaponInfo);
		DroppedWeapon->FinishSpawning(SpawnTransform);
	}
}

void UP13InventoryComponent::Multicast_OnCurrentWeaponUpdatedBroadcast_Implementation(const FP13WeaponSlot& NewWeaponSlot, const int32 CurrentIndex)
{
	OnCurrentWeaponUpdated.Broadcast(NewWeaponSlot, CurrentIndex);
}

void UP13InventoryComponent::Multicast_OnSwitchWeaponBroadcast_Implementation(const int32 NewWeaponIndex, AP13Weapon* NewWeapon)
{
	OnSwitchWeapon.Broadcast(NewWeaponIndex, NewWeapon);
}

void UP13InventoryComponent::Multicast_OnAmmoChangedBroadcast_Implementation(EP13AmmoType AmmoType, int32 NewWeaponAmmoCount, int32 NewInventoryAmmoCount)
{
	OnAmmoChanged.Broadcast(AmmoType, NewWeaponAmmoCount, NewInventoryAmmoCount);
}

void UP13InventoryComponent::Multicast_OnInventoryUpdatedBroadcast_Implementation()
{
	OnInventoryUpdated.Broadcast();
}

void UP13InventoryComponent::Multicast_OnNewWeaponTakenBroadcast_Implementation(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot)
{
	OnNewWeaponTaken.Broadcast(NewWeaponIndex, NewWeaponSlot);
}

void UP13InventoryComponent::Multicast_OnNewAmmoTakenBroadcast_Implementation(const FP13AmmoSlot& NewAmmoSlot)
{
	OnNewAmmoTaken.Broadcast(NewAmmoSlot);
}

void UP13InventoryComponent::Server_TakingAmmoToInventoryAttempt_Implementation(const FP13AmmoSlot& NewAmmoSlot, AP13PickingUpBase* PickupActor)
{
	TakingAmmoToInventoryAttempt(NewAmmoSlot, PickupActor);
}

void UP13InventoryComponent::Server_TakingWeaponToInventoryAttempt_Implementation(const FP13AmmoSlot& NewAmmoSlot, const FP13WeaponSlot& NewWeaponSlot, AP13PickingUpBase* PickupActor)
{
	TakingWeaponToInventoryAttempt(NewAmmoSlot, NewWeaponSlot, PickupActor);
}
