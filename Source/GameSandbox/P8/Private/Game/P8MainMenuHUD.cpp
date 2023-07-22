// Copyright (C) 2023, IKinder

#include "P8/Public/Game/P8MainMenuHUD.h"
#include "Blueprint/UserWidget.h"
#include "P8/Public/Widget/P8MainMenuWidget.h"
// #include "P8/Public/Game/P8GameInstance.h"

AP8MainMenuHUD::AP8MainMenuHUD()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP8MainMenuHUD::BeginPlay()
{
	Super::BeginPlay();

	// if (const UP8GameInstance* GameInstance = Cast<UP8GameInstance>(GetWorld()->GetGameInstance()))
	// {
	// 	GameInstance->LoadMenu();
	// }

	if (MainMenuWidgetClass)
	{
		MainMenuWidget = CreateWidget<UP8MainMenuWidget>(GetWorld(), MainMenuWidgetClass);
		if (!MainMenuWidget) return;
		MainMenuWidget->AddToViewport();
	}
}

void AP8MainMenuHUD::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

