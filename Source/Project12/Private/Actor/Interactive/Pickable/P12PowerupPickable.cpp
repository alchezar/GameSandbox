// Copyright (C) 2023, IKinder

#include "Actor/Interactive/Pickable/P12PowerupPickable.h"

#include "Inventory/P12InventoryItem.h"
#include "Player/P12BaseCharacter.h"
#include "Util/P12DataTableUtil.h"

AP12PowerupPickable::AP12PowerupPickable()
{
	PowerupMesh = CreateDefaultSubobject<UStaticMeshComponent>("PowerupStaticMeshComponent");
	SetRootComponent(PowerupMesh);
}

void AP12PowerupPickable::Interact(AP12BaseCharacter* Char)
{
	const FP12ItemTableRow* ItemRow = P12DataTableUtil::FindInventoryItemData(DataTableID);
	if (!ItemRow)
	{
		return;
	}
	/* Some disadvantages of the current architecture - we need an instance of the object before we know about possible pickup. */
	auto Item = TWeakObjectPtr<UP12InventoryItem>(NewObject<UP12InventoryItem>(Char, ItemRow->InventoryItemClass));
	Item->Initialize(DataTableID, ItemRow->InventoryItemDescription);
	if (Char->PickupItem(Item))
	{
		Destroy();
	}
}

FName AP12PowerupPickable::GetActionEventName() const
{
	return FName("Interact_Powerup");
}
