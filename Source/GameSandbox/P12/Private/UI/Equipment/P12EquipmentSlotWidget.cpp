// Copyright (C) 2023, IKinder

#include "P12/Public/UI/Equipment/P12EquipmentSlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "P12/Public/Actor/Equipment/P12EquipableItem.h"
#include "P12/Public/Inventory/Equipable/P12WeaponInventoryItem.h"
#include "P12/Public/UI/Inventory/P12InventorySlotWidget.h"
#include "P12/Public/Util/P12DataTableUtil.h"

void UP12EquipmentSlotWidget::InitializeSlot(AP12EquipableItem* InEquipableItem, const int32 InItemIndex)
{
	if (!InEquipableItem)
	{
		return;
	}
	LinkedEquipableItem = InEquipableItem;
	SlotIndexInComponent = InItemIndex;

	if (const FP12WeaponTableRow* WeaponTableRow = P12DataTableUtil::FindWeaponData(InEquipableItem->GetDataTableID()))
	{
		InventoryItemAdapter = NewObject<UP12WeaponInventoryItem>(InEquipableItem->GetOwner());
		InventoryItemAdapter->Initialize(InEquipableItem->GetDataTableID(), WeaponTableRow->WeaponItemDescription);
		InventoryItemAdapter->SetEquipWeaponClass(WeaponTableRow->EquipableActor);
	}
}

void UP12EquipmentSlotWidget::UpdateView()
{
	const bool bValid = LinkedEquipableItem.IsValid();
	ItemIconImage->SetBrushFromTexture(bValid ? InventoryItemAdapter->GetDescription().Icon : nullptr);
	ItemNameText->SetText(bValid ? InventoryItemAdapter->GetDescription().Name : FText::FromName(NAME_None));
}

FReply UP12EquipmentSlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	if (!LinkedEquipableItem.IsValid())
	{
		return FReply::Handled();
	}
	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return Reply.NativeReply;
}

void UP12EquipmentSlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	if (!DragAndDropWidgetClass.Get() || !InventoryItemAdapter.IsValid())
	{
		return;
	}
	UP12InventorySlotWidget* DragWidget = CreateWidget<UP12InventorySlotWidget>(GetOwningPlayer(), DragAndDropWidgetClass);
	check(DragWidget)
	DragWidget->SetItemIcon(InventoryItemAdapter->GetDescription().Icon);

	UDragDropOperation* DragOperation = NewObject<UDragDropOperation>();
	DragOperation->DefaultDragVisual = DragWidget;
	DragOperation->Pivot = EDragPivot::MouseDown;
	DragOperation->Payload = InventoryItemAdapter.Get();
	OutOperation = DragOperation;

	LinkedEquipableItem.Reset();
	OnEquipmentRemoveFromSlot.Broadcast(SlotIndexInComponent);
	UpdateView();
}

bool UP12EquipmentSlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const UP12WeaponInventoryItem* InventoryItem = Cast<UP12WeaponInventoryItem>(InOperation->Payload);
	if (!InventoryItem)
	{
		return false;
	}
	return OnEquipmentDropInSlot.Execute(InventoryItem->GetEquipWeaponClass(), SlotIndexInComponent);
}

void UP12EquipmentSlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	InventoryItemAdapter = Cast<UP12WeaponInventoryItem>(InOperation->Payload);
	check(InventoryItemAdapter.IsValid())

	// LinkedEquipableItem = InventoryItemAdapter->GetEquipWeaponClass().Get();
	// UpdateView();

	OnEquipmentDropInSlot.Execute(InventoryItemAdapter->GetEquipWeaponClass(), SlotIndexInComponent);
}
