// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12EquipmentSlotWidget.generated.h"

class UP12WeaponInventoryItem;
class UP12InventorySlotWidget;
class AP12EquipableItem;
class UImage;
class UTextBlock;

UCLASS()
class PROJECT12_API UP12EquipmentSlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DELEGATE_RetVal_TwoParams(bool, FP12OnEquipmentDropInSlotSignature, const TSubclassOf<AP12EquipableItem>& /*Class*/, int32 /*SlotIndex*/)
	DECLARE_MULTICAST_DELEGATE_OneParam(FP12OnEquipmentRemoveFromSlotSignature, int32 /* SlotIndex */)
	FP12OnEquipmentDropInSlotSignature OnEquipmentDropInSlot;
	FP12OnEquipmentRemoveFromSlotSignature OnEquipmentRemoveFromSlot;

	void InitializeSlot(AP12EquipableItem* InEquipableItem, const int32 InItemIndex);
	void UpdateView();

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIconImage;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameText;

	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP12InventorySlotWidget> DragAndDropWidgetClass;

private:
	TWeakObjectPtr<UP12WeaponInventoryItem> InventoryItemAdapter;
	TWeakObjectPtr<AP12EquipableItem> LinkedEquipableItem;
	int32 SlotIndexInComponent = 0;
};
