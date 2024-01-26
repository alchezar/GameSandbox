// Copyright © 2024, IKinder

#include "P13/Public/Game/P13HeadsUpDisplay.h"

#include "Blueprint/UserWidget.h"
#include "P13/Public/UI/Game/P13EndGameWidget.h"
#include "P13/Public/UI/Game/P13InGameWidget.h"

AP13HeadsUpDisplay::AP13HeadsUpDisplay() {}

void AP13HeadsUpDisplay::BeginPlay()
{
	Super::BeginPlay();
}

void AP13HeadsUpDisplay::ShowInGame(APlayerController* PlayerControllerOwner)
{
	if (!InGameWidgetClass || InGameWidgetCached.IsValid())
	{
		return;
	}
	InGameWidgetCached = CreateWidget<UP13InGameWidget>(PlayerControllerOwner, InGameWidgetClass);
	check(InGameWidgetCached.IsValid())
	InGameWidgetCached->AddToViewport();
}

void AP13HeadsUpDisplay::ShowEndGame(const bool bWin)
{
	if (!EndGameWidgetClass)
	{
		return;
	}

	ClearInGame();

	UP13EndGameWidget* EndGameWidget = CreateWidget<UP13EndGameWidget>(GetOwningPlayerController(), EndGameWidgetClass);
	check(EndGameWidget)
	EndGameWidget->InitWidget(bWin);
	EndGameWidget->AddToViewport(1);
}

void AP13HeadsUpDisplay::ClearInGame()
{
	if (!InGameWidgetCached.IsValid())
	{
		return;
	}

	InGameWidgetCached->RemoveInGameWidget();
	InGameWidgetCached->RemoveFromParent();
	InGameWidgetCached.Reset();
}
