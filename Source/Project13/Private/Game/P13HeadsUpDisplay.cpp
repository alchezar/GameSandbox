// Copyright © 2024, IKinder

#include "Game/P13HeadsUpDisplay.h"

#include "Blueprint/UserWidget.h"
#include "UI/Game/P13EndGameWidget.h"
#include "UI/Game/P13InGameWidget.h"

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

void AP13HeadsUpDisplay::ShowEndGame(const bool bWin, APlayerController* PlayerControllerOwner)
{
	if (!EndGameWidgetClass || !PlayerControllerOwner || GetWorld()->bIsTearingDown)
	{
		return;
	}

	ClearInGame();

	UP13EndGameWidget* EndGameWidget = CreateWidget<UP13EndGameWidget>(PlayerControllerOwner, EndGameWidgetClass);
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
