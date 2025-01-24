// Copyright (C) 2023, IKinder

#include "UI/MainMenu/P12HostSessionWidget.h"

#include "Components/Button.h"
#include "Components/EditableTextBox.h"
#include "Components/TextBlock.h"
#include "Game/P12GameInstance.h"

void UP12HostSessionWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ToggleLeftButton->OnClicked.AddDynamic(this, &ThisClass::OnToggleLeftButtonClicked);
	ToggleRightButton->OnClicked.AddDynamic(this, &ThisClass::OnToggleRightButtonClicked);
	UserServerNameTextBox->OnTextCommitted.AddDynamic(this, &ThisClass::OnUserServerNameCommitted);
	BackButton->OnClicked.AddDynamic(this, &ThisClass::OnBackButtonClicked);
	CreateButton->OnClicked.AddDynamic(this, &ThisClass::OnCreateButtonClicked);
	UpdateNetworkTypeText();
}

void UP12HostSessionWidget::CreateSession()
{
	UP12GameInstance* GameInstance = GetWorld()->GetGameInstance<UP12GameInstance>();
	check(GameInstance);
	GameInstance->LaunchLobby(4, ServerName, bLAN);
}

void UP12HostSessionWidget::UpdateNetworkTypeText()
{
	NetworkTypeText->SetText(GetNetworkType());
}

void UP12HostSessionWidget::ToggleNetworkType()
{
	Super::ToggleNetworkType();
	UpdateNetworkTypeText();
}

void UP12HostSessionWidget::OnToggleLeftButtonClicked()
{
	ToggleNetworkType();
}

void UP12HostSessionWidget::OnToggleRightButtonClicked()
{
	ToggleNetworkType();
}

void UP12HostSessionWidget::OnUserServerNameCommitted(const FText& Text, ETextCommit::Type CommitMethod)
{
	ServerName = *Text.ToString();
}

void UP12HostSessionWidget::OnBackButtonClicked()
{
	CloseWidget();
}

void UP12HostSessionWidget::OnCreateButtonClicked()
{
	CreateSession();
}
