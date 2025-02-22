// Copyright (C) 2023, IKinder

#include "UI/P11HUD.h"

#include "Blueprint/UserWidget.h"
#include "Game/P11GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/P11MainMenu.h"
#include "UI/P11MainWidget.h"
#include "UI/P11MenuFPS.h"
#include "UI/P11StatScoreboard.h"
#include "UI/Chat/P11ChatOnScreen.h"

DEFINE_LOG_CATEGORY_STATIC(LogP11HUD, All, All)

AP11HUD::AP11HUD()
{
	
}

void AP11HUD::BeginPlay()
{
	Super::BeginPlay();
	
	ShowFPS();
	ShowChat(false);
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
	MainWidget->AddToViewport(0);
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
		MainMenu->AddToViewport(2);
		MainMenu->SetVisibility(ESlateVisibility::Collapsed);
	}
	MainMenu->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void AP11HUD::ShowScore(const bool bVisible)
{
	if (!ScoreboardClass)
	{
		UE_LOG(LogP11HUD, Warning, TEXT("Scoreboard class not assigned!"));
		return;
	}
	if (!Scoreboard && bVisible)
	{
		Scoreboard = CreateWidget<UP11StatScoreboard>(PlayerOwner, ScoreboardClass);
		if (!Scoreboard)
		{
			UE_LOG(LogP11HUD, Warning, TEXT("No Scoreboard widget!"))
			return;
		}
		Scoreboard->AddToViewport(1);
		Scoreboard->SetVisibility(ESlateVisibility::Collapsed);
	}
	Scoreboard->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void AP11HUD::ShowChat(const bool bVisible)
{
	if (!ChatClass)
	{
		UE_LOG(LogP11HUD, Warning, TEXT("Chat class not assigned!"));
		return;
	}
	if (!ChatOnScreen)
	{
		ChatOnScreen = CreateWidget<UP11ChatOnScreen>(PlayerOwner, ChatClass);
		if (!ChatOnScreen)
		{
			UE_LOG(LogP11HUD, Warning, TEXT("No Chat widget!"))
			return;
		}
		ChatOnScreen->AddToViewport(1);
		ChatOnScreen->SetVisibility(ESlateVisibility::Visible);
	}
	// Chat->SetVisibility(bVisible ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	ChatOnScreen->ExtendChat(bVisible);
}
