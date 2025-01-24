// Copyright (C) 2023, IKinder

#include "Util/P12DataTableUtil.h"

#include "Inventory/P12InventoryItem.h"

FP12WeaponTableRow* P12DataTableUtil::FindWeaponData(const FName& WeaponID)
{
	const FString ContextString = ("Find weapon data");
	const UDataTable* WeaponDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Project/PP12/Data/DataTable/DT_WeaponList.DT_WeaponList'"));

	return !WeaponDataTable ? nullptr : WeaponDataTable->FindRow<FP12WeaponTableRow>(WeaponID, ContextString);
}

FP12ItemTableRow* P12DataTableUtil::FindInventoryItemData(const FName& ItemID)
{
	const FString ContextString = ("Find inventory item data");
	const UDataTable* InventoryItemDataTable = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Project/PP12/Data/DataTable/DT_InventoryItemList.DT_InventoryItemList'"));

	return !InventoryItemDataTable ? nullptr : InventoryItemDataTable->FindRow<FP12ItemTableRow>(ItemID, ContextString);
}
