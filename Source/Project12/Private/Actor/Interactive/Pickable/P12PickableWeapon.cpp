// Copyright (C) 2023, IKinder

#include "Actor/Interactive/Pickable/P12PickableWeapon.h"

#include "Inventory/P12InventoryItem.h"
#include "Inventory/Equipable/P12WeaponInventoryItem.h"
#include "Player/P12BaseCharacter.h"
#include "Util/P12DataTableUtil.h"

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
	const TWeakObjectPtr<UP12WeaponInventoryItem> Weapon = TWeakObjectPtr<UP12WeaponInventoryItem>(NewObject<UP12WeaponInventoryItem>(Char));
	Weapon->Initialize(DataTableID, WeaponRow->WeaponItemDescription);
	Weapon->SetEquipWeaponClass(WeaponRow->EquipableActor);
	Char->PickupItem(Weapon);

	Destroy();
}

FName AP12PickableWeapon::GetActionEventName() const
{
	return FName("Interact_Pickup");
}
