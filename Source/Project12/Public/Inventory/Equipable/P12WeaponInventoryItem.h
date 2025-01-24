// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Inventory/P12InventoryItem.h"
#include "P12WeaponInventoryItem.generated.h"

class AP12EquipableItem;

UCLASS()
class PROJECT12_API UP12WeaponInventoryItem : public UP12InventoryItem
{
	GENERATED_BODY()

public:
	UP12WeaponInventoryItem();
	FORCEINLINE TSubclassOf<AP12EquipableItem> GetEquipWeaponClass() const { return EquipWeaponClass; }
	void SetEquipWeaponClass(const TSubclassOf<AP12EquipableItem>& InEquipWeaponClass);

protected:
	TSubclassOf<AP12EquipableItem> EquipWeaponClass;
};
