// Copyright (C) 2023, IKinder

#include "Game/P8MainGameHUD.h"

#include "Widget/P8InGameMenuWidget.h"

AP8MainGameHUD::AP8MainGameHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP8MainGameHUD::BeginPlay()
{
	Super::BeginPlay();
	check(PauseMenuClass)
}

void AP8MainGameHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

UP8InGameMenuWidget* AP8MainGameHUD::ShowPauseMenu(const bool bVisibility)
{
	if (PauseMenuWidget)
	{
		PauseMenuWidget->SetVisibility(bVisibility ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
		return PauseMenuWidget;
	}
	if (bVisibility && !PauseMenuWidget)
	{
		PauseMenuWidget = CreateWidget<UP8InGameMenuWidget>(GetWorld(), PauseMenuClass);
		if (!PauseMenuWidget) return nullptr;
		PauseMenuWidget->AddToViewport();
	}
	return PauseMenuWidget;
}