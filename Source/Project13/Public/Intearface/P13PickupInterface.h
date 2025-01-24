// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Library/P13Types.h"
#include "UObject/Interface.h"
#include "P13PickupInterface.generated.h"

class AP13PickingUpBase;
struct FP13WeaponSlot;
struct FP13AmmoSlot;

UINTERFACE()
class UP13PickupInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT13_API IP13PickupInterface
{
	GENERATED_BODY()

public:
	virtual void PickupAmmoToInventory(const FP13AmmoSlot& NewAmmoSlot, AP13PickingUpBase* PickupActor) = 0;
	virtual void PickupWeaponToInventory(const FP13AmmoSlot& NewAmmoSlot, const FP13WeaponSlot& NewWeaponSlot, AP13PickingUpBase* PickupActor) = 0;
};
