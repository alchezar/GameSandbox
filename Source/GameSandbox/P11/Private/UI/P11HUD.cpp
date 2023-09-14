// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11HUD.h"

#include "Blueprint/UserWidget.h"
#include "P11/Public/UI/P11MainMenu.h"
#include "P11/Public/UI/P11MainWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogP11HUD, All, All)

AP11HUD::AP11HUD()
{
	
}

void AP11HUD::BeginPlay()
{
	Super::BeginPlay();
}

void AP11HUD::ShowUI(const bool bVisible)
{
	if (!MainWidgetClass)
	{
		return;
	}
	if (!bVisible || bVisible && MainWidget)
	{
		MainWidget->RemoveFromParent();
		MainWidget = nullptr;
		return;
	}
	MainWidget = CreateWidget<UP11MainWidget>(PlayerOwner, MainWidgetClass);
	if (!MainWidget)
	{
		UE_LOG(LogP11HUD, Warning, TEXT("No Main UI Widget"));
		return;
	}
	MainWidget->AddToViewport();
}

void AP11HUD::ShowMainMenu(const bool bVisible)
{
	if (!MainMenuClass)
	{
		return;
	}
	if (!MainMenu && bVisible)
	{
		MainMenu = CreateWidget<UP11MainMenu>(PlayerOwner, MainMenuClass);
		if (!MainMenu)
		{
			UE_LOG(LogP11HUD, Warning, TEXT("No Main Menu Widget"));
			return;
		}
		MainMenu->AddToViewport();
		MainMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
	MainMenu->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}
