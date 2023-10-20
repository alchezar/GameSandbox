// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P12/Public/Util/P12CoreTypes.h"
#include "P12EquipmentComponent.generated.h"

class AP12BaseCharacter;
class AP12RangeWeaponItem;


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12EquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UP12EquipmentComponent();
	EP12EquipablItemType GetCurrentEquippedItemType() const;

protected:
	virtual void BeginPlay() override;

private:
	void CreateLoadout();
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	TSubclassOf<AP12RangeWeaponItem> SideArmClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Loadout")
	FName SideArmSocketName = "GripPoint";
	
private:
	TWeakObjectPtr<AP12BaseCharacter> CachedCharacter;
	UPROPERTY()
	AP12RangeWeaponItem* CurrentEquippedItem = nullptr;
};
