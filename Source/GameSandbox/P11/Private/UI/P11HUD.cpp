// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11HUD.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "P11/Public/Game/P11GameInstance.h"
#include "P11/Public/UI/P11MainMenu.h"
#include "P11/Public/UI/P11MainWidget.h"
#include "P11/Public/UI/P11MenuFPS.h"

DEFINE_LOG_CATEGORY_STATIC(LogP11HUD, All, All)

AP11HUD::AP11HUD()
{
	
}

void AP11HUD::BeginPlay()
{
	Super::BeginPlay();
	
	ShowFPS();
}

void AP11HUD::ShowFPS()
{
	if (!MenuFpsClass)
	{
		UE_LOG(LogP11HUD, Warning, TEXT("No MenuFpsClass added!"));
		return;
	}
	if (!MenuFPS)
	{
		MenuFPS = CreateWidget<UP11MenuFPS>(PlayerOwner, MenuFpsClass);
		check(MenuFPS)
		MenuFPS->AddToViewport();
		MenuFPS->SetVisibility(ESlateVisibility::Collapsed);
	}
	const auto GameInstance = Cast<UP11GameInstance>(GetGameInstance());
	check(GameInstance)
	const auto SavedInfo = Cast<UP11SavePlayerInfo>(UGameplayStatics::LoadGameFromSlot(GameInstance->GetSlotName(), 0));
	check(SavedInfo)
	MenuFPS->SetVisibility(SavedInfo->bShowFPS ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
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
