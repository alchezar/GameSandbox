// Copyright (C) 2023, IKinder

#include "P12/Public/Game/P12HUD.h"

#include "P12/Public/UI/P12PlayerHUDWidget.h"

AP12HUD::AP12HUD()
{	
}

void AP12HUD::BeginPlay()
{
	Super::BeginPlay();
	ShowGameScreen();
}

void AP12HUD::ShowGameScreen()
{
	if (!GameScreenClass)
	{
		return;
	}
	UP12PlayerHUDWidget* GameScreenWidget = CreateWidget<UP12PlayerHUDWidget>(GetWorld(), GameScreenClass);
	if (!GameScreenWidget)
	{
		return;
	}
	GameScreenWidget->AddToViewport();
}
