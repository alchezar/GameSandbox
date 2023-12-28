// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameInstance.h"

FP13WeaponInfo* UP13GameInstance::GetWeaponInfoByID(const FName WeaponID) const
{
	if (!WeaponInfoTable)
	{
		return nullptr;
	}
	return WeaponInfoTable->FindRow<FP13WeaponInfo>(WeaponID, nullptr);
}
