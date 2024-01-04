// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P13/Public/Library/P13Types.h"
#include "UObject/Interface.h"
#include "P13PickupInterface.generated.h"

struct FP13WeaponSlot;
struct FP13AmmoSlot;

UINTERFACE()
class UP13PickupInterface : public UInterface
{
	GENERATED_BODY()
};

class GAMESANDBOX_API IP13PickupInterface
{
	GENERATED_BODY()

public:
	virtual bool CheckCanTakeAmmo(const FP13AmmoSlot& NewAmmoSlot) = 0;
	virtual bool CheckCanTakeWeapon(const FP13WeaponSlot& WeaponSlot) = 0;
	virtual void SaveItemToInventory() = 0;
	virtual void TakeAmmoToInventory(const FP13AmmoSlot& NewAmmoSlot) = 0;
	virtual void TakeWeaponToInventory(const FName NewWeaponID) = 0;
};
