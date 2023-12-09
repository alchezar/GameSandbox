// Copyright (C) 2023, IKinder

#include "P12/Public/UI/MainMenu/P12MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "P12/Public/Game/P12HUD.h"
#include "P12/Public/UI/MainMenu/P12HostSessionWidget.h"
#include "P12/Public/UI/MainMenu/P12JoinSessionWidget.h"

void UP12MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnButton->OnClicked.AddDynamic(this, &ThisClass::OnReturnButtonClickedHandle);
	HostButton->OnClicked.AddDynamic(this, &ThisClass::OnHostButtonClickedHandle);
	JoinButton->OnClicked.AddDynamic(this, &ThisClass::OnJoinButtonClickedHandle);
	HostSessionWidget->OnNetworkWidgetClose.AddUObject(this, &ThisClass::OnNetworkWidgetClosedHandle);
	JoinSessionWidget->OnNetworkWidgetClose.AddUObject(this, &ThisClass::OnNetworkWidgetClosedHandle);
}

void UP12MainMenuWidget::CacheHUD(const AP12HUD* HUD)
{
	CachedHUD = HUD;
}

void UP12MainMenuWidget::OnReturnButtonClickedHandle()
{
	CachedHUD->ShowMainMenu(false);
}

void UP12MainMenuWidget::OnHostButtonClickedHandle()
{
	MenuSwitcher->SetActiveWidgetIndex(CreateSessionIndex);
}

void UP12MainMenuWidget::OnJoinButtonClickedHandle()
{
	MenuSwitcher->SetActiveWidgetIndex(JoinSessionIndex);
}

void UP12MainMenuWidget::OnNetworkWidgetClosedHandle()
{
	MenuSwitcher->SetActiveWidgetIndex(MainMenuIndex);
}
