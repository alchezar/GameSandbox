// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "P12InventoryItem.generated.h"

class AP12EquipableItem;
class AP12PickableItem;

USTRUCT(BlueprintType)
struct FP12InventoryItemDescription : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FText Name;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* Icon;
};

USTRUCT(BlueprintType)
struct FP12WeaponTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AP12PickableItem> PickableActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AP12EquipableItem> EquipableActor;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FP12InventoryItemDescription WeaponItemDescription;
};

UCLASS()
class GAMESANDBOX_API UP12InventoryItem : public UObject
{
	GENERATED_BODY()
};
