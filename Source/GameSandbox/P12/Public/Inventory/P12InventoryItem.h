// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "P12InventoryItem.generated.h"

class UP12InventoryItem;
class AP12BaseCharacter;
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

USTRUCT(BlueprintType)
struct FP12ItemTableRow : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AP12PickableItem> PickableActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UP12InventoryItem> InventoryItemClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FP12InventoryItemDescription InventoryItemDescription;
};

UENUM(BlueprintType)
enum class EP12ItemType : uint8
{
	None = 0,
	Equipable,
	Consumable
};

UCLASS(Blueprintable)
class GAMESANDBOX_API UP12InventoryItem : public UObject
{
	GENERATED_BODY()

public:
	void Initialize(const FName& InDataTableID, const FP12InventoryItemDescription& InDescription);
	FORCEINLINE FName GetDataTableID() const { return DataTableID; }
	FORCEINLINE const FP12InventoryItemDescription& GetDescription() const { return Description; }
	virtual bool GetIsEquipable() const;
	virtual bool GetIsConsumable() const;
	virtual bool Consume(AP12BaseCharacter* ConsumeTarget) PURE_VIRTUAL(UP12InventoryItem::Consume, return false;);

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Inventory")
	FName DataTableID = NAME_None;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Inventory")
	FP12InventoryItemDescription Description;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Inventory")
	EP12ItemType ItemType = EP12ItemType::None;

private:
	bool bInitialized = false;
};
