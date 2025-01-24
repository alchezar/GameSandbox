// Copyright (C) 2023, IKinder

#include "UI/MainMenu/P12JoinSessionWidget.h"

#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Components/WidgetSwitcher.h"
#include "Game/P12GameInstance.h"

void UP12JoinSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CachedGameInstance = GetWorld()->GetGameInstance<UP12GameInstance>();
	ToggleLeftButton->OnClicked.AddDynamic(this, &ThisClass::OnToggleSideButtonClicked);
	ToggleRightButton->OnClicked.AddDynamic(this, &ThisClass::OnToggleSideButtonClicked);
	BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
	AcceptButton->OnClicked.AddDynamic(this, &ThisClass::OnAcceptButtonClicked);
	UpdateNetworkTypeText();
}

void UP12JoinSessionWidget::CloseWidget()
{
	CachedGameInstance->OnMatchFound.RemoveAll(this);

	Super::CloseWidget();
}

void UP12JoinSessionWidget::FindOnlineSession()
{
	CachedGameInstance->OnMatchFound.AddUObject(this, &ThisClass::OnMatchFound);
	CachedGameInstance->FindMatch(bLAN);
	SearchingSessionState = EP12SearchingSessionState::Searching;
}

void UP12JoinSessionWidget::JoinOnlineSession()
{
	CachedGameInstance->JoinOnlineGame();
}

void UP12JoinSessionWidget::OnMatchFound(bool bSuccessful)
{
	SearchingSessionState = bSuccessful ? EP12SearchingSessionState::Found : EP12SearchingSessionState::None;
	CachedGameInstance->OnMatchFound.RemoveAll(this);

	ToggleWidgets->SetActiveWidgetIndex(static_cast<int32>(SearchingSessionState));
}

void UP12JoinSessionWidget::UpdateNetworkTypeText()
{
	NetworkTypeText->SetText(GetNetworkType());
}

void UP12JoinSessionWidget::OnToggleSideButtonClicked()
{
	ToggleNetworkType();
	UpdateNetworkTypeText();
}

void UP12JoinSessionWidget::OnBackButtonClicked()
{
	CloseWidget();
}

void UP12JoinSessionWidget::OnAcceptButtonClicked()
{
	if (SearchingSessionState == EP12SearchingSessionState::None)
	{
		FindOnlineSession();
		ToggleWidgets->SetActiveWidgetIndex(1);
	}
	if (SearchingSessionState == EP12SearchingSessionState::Found)
	{
		JoinOnlineSession();
	}
}
