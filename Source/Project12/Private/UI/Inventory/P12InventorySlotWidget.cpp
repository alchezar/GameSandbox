// Copyright (C) 2023, IKinder

#include "UI/Inventory/P12InventorySlotWidget.h"

#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Component/Actor/P12InventoryComponent.h"
#include "Components/Image.h"
#include "Input/Reply.h"
#include "Inventory/P12InventoryItem.h"
#include "Player/P12BaseCharacter.h"

void UP12InventorySlotWidget::InitializeSlot(FP12InventorySlot& InventorySlot)
{
	LinkedSlot = &InventorySlot;
	FP12InventorySlot::FP12OnInventorySlotUpdateSignature OnInventorySlotUpdate;
	OnInventorySlotUpdate.AddUObject(this, &ThisClass::UpdateView);
	LinkedSlot->BindOnInventorySlotUpdate(OnInventorySlotUpdate);
}

void UP12InventorySlotWidget::UpdateView()
{
	const bool bValid = LinkedSlot && LinkedSlot->Item.IsValid();
	ItemIcon->SetBrushFromTexture(bValid ? LinkedSlot->Item->GetDescription().Icon : nullptr);
}

void UP12InventorySlotWidget::SetItemIcon(UTexture2D* InIcon)
{
	ItemIcon->SetBrushFromTexture(InIcon);
}

FReply UP12InventorySlotWidget::NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	// return Super::NativeOnMouseButtonDown(InGeometry, InMouseEvent);

	if (!LinkedSlot || (LinkedSlot && !LinkedSlot->Item.IsValid()))
	{
		return FReply::Handled();
	}
	const FKey MouseButton = InMouseEvent.GetEffectingButton();
	if (MouseButton == EKeys::RightMouseButton)
	{
		/* Some simplification in order not to complicate the architecture.
		 * When creating them, we use the current pawn as an outer one.
		 * In real practice, we need to use a callback to tell the item owner what action was performed in the UI.*/
		const TWeakObjectPtr<UP12InventoryItem> LinkedSlotItem = LinkedSlot->Item;
		AP12BaseCharacter* ItemOwner = Cast<AP12BaseCharacter>(LinkedSlotItem->GetOuter());
		if (LinkedSlotItem->Consume(ItemOwner))
		{
			LinkedSlot->ClearSlot();
		}
		return FReply::Handled();
	}

	FEventReply Reply = UWidgetBlueprintLibrary::DetectDragIfPressed(InMouseEvent, this, EKeys::LeftMouseButton);
	return Reply.NativeReply;
}

void UP12InventorySlotWidget::NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation)
{
	// UDragDropOperation* DragOperation = Cast<UDragDropOperation>(UWidgetBlueprintLibrary::CreateDragDropOperation(UDragDropOperation::StaticClass()));
	UDragDropOperation* DragOperation = NewObject<UDragDropOperation>();

	/* Some simplification to not define a new widget for the drag and drop operation. */
	UP12InventorySlotWidget* DragWidget = CreateWidget<UP12InventorySlotWidget>(GetOwningPlayer(), GetClass());
	DragWidget->ItemIcon->SetBrushFromTexture(LinkedSlot->Item->GetDescription().Icon);

	DragOperation->DefaultDragVisual = DragWidget;
	DragOperation->Pivot = EDragPivot::MouseDown;
	DragOperation->Payload = LinkedSlot->Item.Get();
	OutOperation = DragOperation;

	LinkedSlot->ClearSlot();
}

bool UP12InventorySlotWidget::NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	const bool bResult = !LinkedSlot->Item.IsValid();
	if (bResult)
	{
		LinkedSlot->Item = Cast<UP12InventoryItem>(InOperation->Payload);
		LinkedSlot->UpdateSlotState();
	}
	return bResult;
}

void UP12InventorySlotWidget::NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation)
{
	LinkedSlot->Item = Cast<UP12InventoryItem>(InOperation->Payload);
	LinkedSlot->UpdateSlotState();
}
