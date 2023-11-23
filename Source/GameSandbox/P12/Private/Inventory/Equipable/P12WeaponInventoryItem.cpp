// Copyright (C) 2023, IKinder

#include "P12/Public/Inventory/Equipable/P12WeaponInventoryItem.h"

UP12WeaponInventoryItem::UP12WeaponInventoryItem()
{
	ItemType = EP12ItemType::Consumable;
}

void UP12WeaponInventoryItem::SetEquipWeaponClass(const TSubclassOf<AP12EquipableItem>& InEquipWeaponClass)
{
	EquipWeaponClass = InEquipWeaponClass;
}
