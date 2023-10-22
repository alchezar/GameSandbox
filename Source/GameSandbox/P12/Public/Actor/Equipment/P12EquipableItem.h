// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "P12/Public/Util/P12CoreTypes.h"
#include "P12EquipableItem.generated.h"


UCLASS(Abstract, NotBlueprintable)
class GAMESANDBOX_API AP12EquipableItem : public AActor
{
	GENERATED_BODY()

public:
	EP12EquipableItemType GetItemType() const { return ItemType; };

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	EP12EquipableItemType ItemType = EP12EquipableItemType::None;
};
