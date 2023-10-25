// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P12/Public/Util/P12CoreTypes.h"
#include "P12EquipmentComponent.generated.h"

class AP12BaseCharacter;
class AP12RangeWeaponItem;

typedef TArray<int32, TInlineAllocator<static_cast<uint32>(EP12AmmunitionType::MAX)>> TP12AmmunitionArray;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP12EquipmentComponent();
	FORCEINLINE AP12RangeWeaponItem* GetCurrentEquippedWeapon() const { return CurrentEquippedWeapon; }
	EP12EquipableItemType GetCurrentEquippedItemType() const;
	int32 GetMaxAvailableAmmoAmount(const EP12AmmunitionType AmmoType);
	void DecreaseMaxAvailableAmmoAmount(const EP12AmmunitionType AmmoType, const int32 AmmoToDecrease);
	
protected:
	virtual void BeginPlay() override;

private:
	void CreateLoadout();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TSubclassOf<AP12RangeWeaponItem> SideArmClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	FName SideArmSocketName = "GripPoint";
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TMap<EP12AmmunitionType, int32> MaxAmmunitionAmount;
	
private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
	UPROPERTY()
	AP12RangeWeaponItem* CurrentEquippedWeapon = nullptr;
	TP12AmmunitionArray AmmunitionArray; 
};
