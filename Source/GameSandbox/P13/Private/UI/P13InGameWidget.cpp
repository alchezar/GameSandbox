// Copyright © 2024, IKinder

#include "P13/Public/UI/P13InGameWidget.h"

#include "P13/Public/UI/Inventory/P13InventoryStatsWidget.h"

void UP13InGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ShowStatWidgets();
}

void UP13InGameWidget::ShowStatWidgets() const
{
	if (auto* InventoryStatsWidget = CreateWidget<UP13InventoryStatsWidget>(GetOwningPlayer(), InventoryStatClass))
	{
		InventoryStatsWidget->AddToViewport();
	}
}
