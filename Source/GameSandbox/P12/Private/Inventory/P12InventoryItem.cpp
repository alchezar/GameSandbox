// Copyright (C) 2023, IKinder

#include "P12/Public/Inventory/P12InventoryItem.h"

void UP12InventoryItem::Initialize(const FName& InDataTableID, const FP12InventoryItemDescription& InDescription)
{
	DataTableID = InDataTableID;
	Description.Name = InDescription.Name;
	Description.Icon = InDescription.Icon;
	bInitialized = true;
}

bool UP12InventoryItem::GetIsEquipable() const
{
	return ItemType == EP12ItemType::Equipable;	
}

bool UP12InventoryItem::GetIsConsumable() const
{
	return ItemType == EP12ItemType::Consumable;	
}
