// Copyright © 2024, IKinder

#include "P13/Public/UI/P13InGameWidget.h"

#include "P13/Public/UI/Damage/P13HealthWidget.h"
#include "P13/Public/UI/Inventory/P13InventoryStatsWidget.h"

void UP13InGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	ShowStatWidgets();
	ShowHealthWidget();

	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->OnPossessedPawnChanged.AddDynamic(this, &ThisClass::OnPawnChangedHandle);
	}
}

void UP13InGameWidget::ShowStatWidgets() const
{
	if (auto* InventoryStatsWidget = CreateWidget<UP13InventoryStatsWidget>(GetOwningPlayer(), InventoryStatClass))
	{
		InventoryStatsWidget->AddToViewport();
	}
}

void UP13InGameWidget::ShowHealthWidget() const
{
	if (auto* HealthWidget = CreateWidget<UP13HealthWidget>(GetOwningPlayer(), HealthClass))
	{
		HealthWidget->AddToViewport();
	}
}

void UP13InGameWidget::OnPawnChangedHandle(APawn* OldPawn, APawn* NewPawn)
{
	ShowHealthWidget();
}
