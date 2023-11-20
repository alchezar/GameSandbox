// Copyright (C) 2023, IKinder

#include "P12/Public/Util/P12DataTableUtil.h"

#include "P12/Public/Inventory/P12InventoryItem.h"

FP12WeaponTableRow* P12DataTableUtil::FindWeaponData(const FName& WeaponID)
{
	const FString ContextString = ("Find weapon data");
	const UDataTable* WeaponTableData = LoadObject<UDataTable>(nullptr, TEXT("/Script/Engine.DataTable'/Game/Project/PP12/Data/DataTable/DT_WeaponList.DT_WeaponList'"));

	return !WeaponTableData ? nullptr : WeaponTableData->FindRow<FP12WeaponTableRow>(WeaponID, ContextString);
}
