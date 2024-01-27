// Copyright © 2024, IKinder

#include "P13/Public/UI/Menu/P13LobbyMenuWidget.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/CircularThrobber.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/Intearface/P13NetworkInterface.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Lobby Menu                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13LobbyMenuWidget::NativeConstruct()
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

void UP13LobbyMenuWidget::OnPlayButtonPressed()
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

void UP13LobbyMenuWidget::OnHostButtonClickedHandle()
{
	GameInstanceNetwork->HostSession(2, IsLanCheckBox->IsChecked(), CustomServerName);
}

void UP13LobbyMenuWidget::OnFindButtonClickedHandle()
{
	SearchingThrobber->SetVisibility(ESlateVisibility::Visible);
	ClearSessionsList();
	GameInstanceNetwork->FindSessions(IsLanCheckBox->IsChecked());	
}

void UP13LobbyMenuWidget::OnLanStateChangedHandle(bool bChecked)
{
	
}

void UP13LobbyMenuWidget::OnFindSessionsCompleteHandle(TArray<FOnlineSessionSearchResult> OnlineSessionSearchResults)
{
	ClearSessionsList();

	APlayerController* PlayerController = GetOwningPlayer();
	check(PlayerController)
		
	for(FOnlineSessionSearchResult Result : OnlineSessionSearchResults)
	{
		UP13SessionSelectWidget* SessionWidget = CreateWidget<UP13SessionSelectWidget>(PlayerController, SessionButtonWidgetClass);
		check(SessionWidget)

		SessionWidget->InitSessionButton(Result);
		SessionWidget->OnSessionSelected.AddUObject(this, &ThisClass::OnSessionSelectedHandle);
		SessionWidget->AddToViewport();
		SessionsVerticalBox->AddChild(SessionWidget);
	}
}

void UP13LobbyMenuWidget::OnSessionSelectedHandle(const FOnlineSessionSearchResult& SelectedResult)
{
	SelectedSessionCached = SelectedResult;
	PlayButton->SetIsEnabled(SelectedResult.Session.NumOpenPublicConnections != 0);
}

void UP13LobbyMenuWidget::ClearSessionsList()
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
	const bool bCustomName = SessionInfo.Session.SessionSettings.Get(SERVER_NAME_KEY , CustomServerName);
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
