// Copyright © 2024, IKinder

#include "P13/Public/UI/P13InGameWidget.h"

#include "P13/Public/UI/Damage/P13HealthWidget.h"
#include "P13/Public/UI/Inventory/P13InventoryStatsWidget.h"

void UP13InGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ShowStatWidgets();
	ShowHealthWidget();
}

void UP13InGameWidget::HideInGameWidgets()
{
	InventoryStatsWidget->SetVisibility(ESlateVisibility::Collapsed);
	HealthWidget->SetVisibility(ESlateVisibility::Collapsed);
}

void UP13InGameWidget::RemoveInGameWidget()
{
	InventoryStatsWidget->RemoveFromParent();
	HealthWidget->RemoveFromParent();
}

void UP13InGameWidget::ShowStatWidgets()
{
	InventoryStatsWidget = CreateWidget<UP13InventoryStatsWidget>(this, InventoryStatClass);
	if (!InventoryStatsWidget)
	{
		return;
	}
	InventoryStatsWidget->AddToViewport();
}

void UP13InGameWidget::ShowHealthWidget()
{
	HealthWidget = CreateWidget<UP13HealthWidget>(this, HealthClass);
	if (!HealthWidget)
	{
		return;
	}
	HealthWidget->AddToViewport();
}
