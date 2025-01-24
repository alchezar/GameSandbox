// Copyright (C) 2023, IKinder

#include "UI/Inventory/P12InventoryViewWidget.h"

#include "Component/Actor/P12InventoryComponent.h"
#include "Components/GridPanel.h"
#include "UI/Inventory/P12InventorySlotWidget.h"

void UP12InventoryViewWidget::InitializeViewWidget(TArray<FP12InventorySlot>& InventorySlots)
{
	for (FP12InventorySlot& Item : InventorySlots)
	{
		AddItemSlotView(Item);
	}
}

void UP12InventoryViewWidget::AddItemSlotView(FP12InventorySlot& SlotToAdd)
{
	check(InventorySlotWidgetClass)
	UP12InventorySlotWidget* SlotWidget = CreateWidget<UP12InventorySlotWidget>(this, InventorySlotWidgetClass);
	check(SlotWidget)

	SlotWidget->InitializeSlot(SlotToAdd);
	const int32 CurrentSlotCount = GridPanelItemsSlot->GetChildrenCount();
	const int32 CurrentSlotRow = CurrentSlotCount / ColumnCount;
	const int32 CurrentSlotColumn = CurrentSlotCount % ColumnCount;

	GridPanelItemsSlot->AddChildToGrid(SlotWidget, CurrentSlotRow, CurrentSlotColumn);
	SlotWidget->UpdateView();
}
