// Copyright (C) 2023, IKinder

#pragma once

struct FP12ItemTableRow;
struct FP12WeaponTableRow;

namespace P12DataTableUtil
{
	FP12WeaponTableRow* FindWeaponData(const FName& WeaponID);
	FP12ItemTableRow* FindInventoryItemData(const FName& ItemID);
}
