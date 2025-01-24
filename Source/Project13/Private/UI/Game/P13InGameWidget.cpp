// Copyright © 2024, IKinder

#include "UI/Game/P13InGameWidget.h"

#include "UI/Damage/P13HealthWidget.h"
#include "UI/Inventory/P13InventoryStatsWidget.h"
#include "UI/Inventory/P13PlayerStatsWidget.h"

void UP13InGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ShowInventoryStatWidget();
	ShowHealthWidget();
	ShowPlayerStatWidget();
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
	PlayerStatWidget->RemoveFromParent();
}

void UP13InGameWidget::ShowInventoryStatWidget()
{
	check(InventoryStatClass)
	InventoryStatsWidget = CreateWidget<UP13InventoryStatsWidget>(this, InventoryStatClass);
	if (!InventoryStatsWidget)
	{
		return;
	}
	InventoryStatsWidget->AddToViewport();
}

void UP13InGameWidget::ShowHealthWidget()
{
	check(HealthClass)
	HealthWidget = CreateWidget<UP13HealthWidget>(this, HealthClass);
	if (!HealthWidget)
	{
		return;
	}
	HealthWidget->AddToViewport();
}

void UP13InGameWidget::ShowPlayerStatWidget()
{
	check(PlayerStatClass)
	PlayerStatWidget = CreateWidget<UP13PlayerStatsWidget>(this, PlayerStatClass);
	if (!PlayerStatWidget)
	{
		return;
	}
	PlayerStatWidget->AddToViewport();
}
