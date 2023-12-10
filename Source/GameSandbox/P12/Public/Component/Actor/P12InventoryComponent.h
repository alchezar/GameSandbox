// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "P12InventoryComponent.generated.h"

class UP12InventoryViewWidget;
class UP12InventoryItem;

USTRUCT(BlueprintType)
struct FP12InventorySlot
{
	GENERATED_BODY()

public:
	DECLARE_MULTICAST_DELEGATE(FP12OnInventorySlotUpdateSignature)

	void BindOnInventorySlotUpdate(const FP12OnInventorySlotUpdateSignature& Callback) const;
	void UnbindOnInventorySlotUpdate(const UObject* Object);
	void UpdateSlotState();
	void ClearSlot();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TWeakObjectPtr<UP12InventoryItem> Item;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count = 0;

private:
	mutable FP12OnInventorySlotUpdateSignature OnInventorySlotUpdate;
};

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAMESANDBOX_API UP12InventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	void OpenViewInventory(APlayerController* InPlayerController);
	void CloseViewInventory();
	bool GetIsViewVisible() const;

	FORCEINLINE int32 GetCapacity() const { return Capacity; }
	FORCEINLINE bool HasFreeSlot() const { return ItemsInInventory < Capacity; }

	bool TryAddItem(const TWeakObjectPtr<UP12InventoryItem> ItemToAdd, int32 Count);
	bool TryRemoveItem(const FName& ItemID);

	FORCEINLINE TArray<FP12InventorySlot> GetAllItemsCopy() const { return InventorySlots; }
	TArray<FText> GetAllItemsNames() const;

protected:
	virtual void BeginPlay() override;

	void CreateViewWidget(APlayerController* InPlayerController);
	FP12InventorySlot* FindItemSlot(FName ItemID);
	FP12InventorySlot* FindFreeSlot();

protected:
	UPROPERTY(EditAnywhere, Category = "C++ | Inventory")
	TArray<FP12InventorySlot> InventorySlots;
	UPROPERTY(EditAnywhere, Category = "C++ | Inventory")
	TSubclassOf<UP12InventoryViewWidget> InventoryViewWidgetClass;
	UPROPERTY(EditAnywhere, Category = "C++ | Inventory", meta = (ClampMin = 1, UIMin = 1))
	int32 Capacity = 16;

private:
	int32 ItemsInInventory;
	UPROPERTY()
	UP12InventoryViewWidget* InventoryViewWidget;
};
