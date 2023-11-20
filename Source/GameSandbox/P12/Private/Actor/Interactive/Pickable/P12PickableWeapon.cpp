// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Interactive/Pickable/P12PickableWeapon.h"

#include "P12/Public/Inventory/P12InventoryItem.h"
#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Util/P12DataTableUtil.h"

AP12PickableWeapon::AP12PickableWeapon()
{
	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponStaticMeshComponent");
	SetRootComponent(WeaponMesh);
}

void AP12PickableWeapon::Interact(AP12BaseCharacter* Char) 
{
	const FP12WeaponTableRow* WeaponRow = P12DataTableUtil::FindWeaponData(DataTableID);
	if (!WeaponRow)
	{
		return;
	}
	Char->AddEquipmentItem(WeaponRow->EquipableActor);
	Destroy();
}

FName AP12PickableWeapon::GetActionEventName() const 
{
	return FName("Interact_Pickup");
}
