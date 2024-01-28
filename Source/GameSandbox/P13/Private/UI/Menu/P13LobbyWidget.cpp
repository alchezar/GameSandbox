// Copyright © 2024, IKinder

#include "P13/Public/UI/Menu/P13LobbyWidget.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/CircularThrobber.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/Game/P13PlayerState.h"
#include "P13/Public/Intearface/P13NetworkInterface.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Start Menu                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13StartMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HostButton->OnReleased.AddDynamic(this, &ThisClass::OnHostButtonClickedHandle);
	FindButton->OnReleased.AddDynamic(this, &ThisClass::OnFindButtonClickedHandle);
	IsLanCheckBox->OnCheckStateChanged.AddDynamic(this, &ThisClass::OnLanStateChangedHandle);

	SearchingThrobber->SetVisibility(ESlateVisibility::Hidden);
	PlayButton->SetIsEnabled(false);

	GameInstanceNetwork = GetWorld()->GetGameInstance<IP13NetworkInterface>();
	check(GameInstanceNetwork)
	GameInstanceNetwork->OnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionsCompleteHandle);
}

void UP13StartMenuWidget::OnPlayButtonPressed()
{
	// Super::OnPlayButtonPressed();

	ClearLevelsList();
	ClearOptionsList();

	if (!SelectedSessionCached.IsValid())
	{
		return;
	}
	GameInstanceNetwork->JoinSession(SelectedSessionCached);
}

void UP13StartMenuWidget::OnHostButtonClickedHandle()
{
	GameInstanceNetwork->HostSession(2, IsLanCheckBox->IsChecked(), CustomServerName);
}

void UP13StartMenuWidget::OnFindButtonClickedHandle()
{
	SearchingThrobber->SetVisibility(ESlateVisibility::Visible);
	ClearSessionsList();
	GameInstanceNetwork->FindSessions(IsLanCheckBox->IsChecked());
}

void UP13StartMenuWidget::OnLanStateChangedHandle(bool bChecked)
{
	
}

void UP13StartMenuWidget::OnFindSessionsCompleteHandle(TArray<FOnlineSessionSearchResult> OnlineSessionSearchResults)
{
	ClearSessionsList();

	APlayerController* PlayerController = GetOwningPlayer();
	check(PlayerController)

	for (FOnlineSessionSearchResult Result : OnlineSessionSearchResults)
	{
		UP13SessionSelectWidget* SessionWidget = CreateWidget<UP13SessionSelectWidget>(PlayerController, SessionButtonWidgetClass);
		check(SessionWidget)

		SessionWidget->InitSessionButton(Result);
		SessionWidget->OnSessionSelected.AddUObject(this, &ThisClass::OnSessionSelectedHandle);
		SessionWidget->AddToViewport();
		SessionsVerticalBox->AddChild(SessionWidget);
	}
}

void UP13StartMenuWidget::OnSessionSelectedHandle(const FOnlineSessionSearchResult& SelectedResult)
{
	SelectedSessionCached = SelectedResult;
	PlayButton->SetIsEnabled(SelectedResult.Session.NumOpenPublicConnections != 0);
}

void UP13StartMenuWidget::ClearSessionsList()
{
	SessionsVerticalBox->ClearChildren();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              Session Select                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13SessionSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SelectButton->OnReleased.AddDynamic(this, &ThisClass::OnSessionButtonPressedHandle);
}

void UP13SessionSelectWidget::InitSessionButton(const FOnlineSessionSearchResult& SessionInfo)
{
	FString CustomServerName;
	const bool bCustomName = SessionInfo.Session.SessionSettings.Get(SERVER_NAME_KEY, CustomServerName);
	const FString ServerName = bCustomName ? SessionInfo.Session.OwningUserName : CustomServerName;
	const int32 MaxPlayersNum = SessionInfo.Session.SessionSettings.NumPublicConnections;
	const int32 CurrentPlayersNum = MaxPlayersNum - SessionInfo.Session.NumOpenPublicConnections;

	ServerNameText->SetText(FText::FromString(ServerName));
	MaxPlayersText->SetText(FText::AsNumber(MaxPlayersNum));
	CurrentPlayersText->SetText(FText::AsNumber(CurrentPlayersNum));
	SelectedResult = SessionInfo;
}

void UP13SessionSelectWidget::OnSessionButtonPressedHandle()
{
	OnSessionSelected.Broadcast(SelectedResult);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Lobby Menu                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13LobbyMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	InitWidget();
}

void UP13LobbyMenuWidget::OnReadyButtonClickedHandle()
{
	
}

void UP13LobbyMenuWidget::OnExitButtonClickedHandle()
{
	if (IP13NetworkInterface* GameInstanceNetwork = GetWorld()->GetGameInstance<IP13NetworkInterface>())
	{
		GameInstanceNetwork->DestroySession();
	}
}

void UP13LobbyMenuWidget::InitWidget()
{
	ReadyButton->OnReleased.AddDynamic(this, &ThisClass::OnReadyButtonClickedHandle);
	ExitButton->OnReleased.AddDynamic(this, &ThisClass::OnExitButtonClickedHandle);

	const APlayerController* OwningController = GetOwningPlayer();
	check(OwningController)
	const AP13PlayerState* PlayerState = OwningController->GetPlayerState<AP13PlayerState>();
	check(PlayerState)
	const bool bServer = OwningController->GetNetMode() != NM_Client;

	const FString NameString = PlayerState->GetPlayerName().Mid(0, 20);
	const FString RoleString = bServer ? "Server" : "Host";
	const FString ReadyString = bServer ? "Start" : "Ready";

	PlayerNameText->SetText(FText::FromString(NameString));
	PlayerRoleText->SetText(FText::FromString(RoleString));
	StartReadyText->SetText(FText::FromString(ReadyString));
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Color Button                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13LevelButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP13LevelButtonWidget::OnColorButtonClickedHandle()
{
	
}
