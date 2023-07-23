// Copyright (C) 2023, IKinder

#include "P8/Public/Game/P8MainMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "P8/Public/Widget/P8MainMenuWidget.h"

AP8MainMenuHUD::AP8MainMenuHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP8MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();
	CreateMainMenu();
}

void AP8MainMenuHUD::CreateMainMenu()
{
	if (!MainMenuWidgetClass) return;
	MainMenuWidget = CreateWidget<UP8MainMenuWidget>(GetWorld(), MainMenuWidgetClass);
	if (!MainMenuWidget) return;
	MainMenuWidget->Setup();
}

void AP8MainMenuHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

