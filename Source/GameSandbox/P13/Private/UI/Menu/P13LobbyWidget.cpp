// Copyright © 2024, IKinder

#include "P13/Public/UI/Menu/P13LobbyWidget.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/CheckBox.h"
#include "Components/CircularThrobber.h"
#include "Components/EditableText.h"
#include "Components/HorizontalBox.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "P13/Public/Controller/P13LobbyPlayerController.h"
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
	SessionNameText->OnTextCommitted.AddDynamic(this, &ThisClass::OnSessionNameCommittedHandle);

	SearchingThrobber->SetVisibility(ESlateVisibility::Hidden);
	PlayButton->SetIsEnabled(false);

	GameInstanceNetwork = GetWorld()->GetGameInstance<IP13NetworkInterface>();
	check(GameInstanceNetwork)
	GameInstanceNetwork->OnFindSessionsComplete.AddUObject(this, &ThisClass::OnFindSessionsCompleteHandle);

	ClearSessionsList();
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

void UP13StartMenuWidget::OnSessionNameCommittedHandle(const FText& Text, ETextCommit::Type CommitMethod)
{
	if (CommitMethod == ETextCommit::Type::OnEnter)
	{
		CustomServerName = Text.ToString();	
	}
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
	FindLevels();
}

void UP13LobbyMenuWidget::UpdateLevelName(const FText& SelectedLevelName) const
{
	SelectedMapText->SetText(SelectedLevelName);
}

void UP13LobbyMenuWidget::SelectLevelName(const FText& SelectedLevelName, const FName SelectedLevelAddress)
{
	if (!bServer)
	{
		return;
	}
	
	CachedLobbyController->OnHostSelectedMap(SelectedLevelName, SelectedLevelAddress);
}

void UP13LobbyMenuWidget::OnReadyButtonClickedHandle()
{
	AP13LobbyPlayerController* LobbyController = GetOwningPlayer<AP13LobbyPlayerController>();
	check(LobbyController)

	if (!bServer)
	{
		LobbyController->Server_UpdateClientReady();
	}
	else if (AP13LobbyGameMode* LobbyGameMode = GetWorld()->GetAuthGameMode<AP13LobbyGameMode>())
	{
		LobbyGameMode->Server_LaunchGame();	
	}
	
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

	check(GetOwningPlayer() && GetOwningPlayer()->IsA<AP13LobbyPlayerController>())
	CachedLobbyController = StaticCast<AP13LobbyPlayerController*>(GetOwningPlayer());
	
	const APlayerState* PlayerState = CachedLobbyController->GetPlayerState<APlayerState>();
	check(PlayerState)
	
	bServer = CachedLobbyController->GetNetMode() != NM_Client;

	int32 StopIndex = 15;
	PlayerState->GetPlayerName().FindChar('-', StopIndex);
	const FString NameString = PlayerState->GetPlayerName().Mid(0, StopIndex);
	const FString RoleString = bServer ? "Server" : "Host";
	const FString ReadyString = bServer ? "Start" : "Ready";

	PlayerNameText->SetText(FText::FromString(NameString));
	PlayerRoleText->SetText(FText::FromString(RoleString));
	StartReadyText->SetText(FText::FromString(ReadyString));

	if (!bServer)
	{
		LevelsHorizontalBox->SetVisibility(ESlateVisibility::Collapsed);
	}
}

void UP13LobbyMenuWidget::FindLevels() const
{
	if (!bServer)
	{
		return;
	}
	ClearLevels();
	
	APlayerController* LobbyController = GetOwningPlayer();
	if (!LobbyController)
	{
		return;
	}
	
	TArray<FP13LevelSelect*> LevelRows;
	LevelSelectTable->GetAllRows(nullptr, LevelRows);
	
	for (int32 Index = 0; Index < LevelRows.Num(); ++Index)
	{
		if (Index == 0)
		{
			UpdateLevelName(LevelRows[0]->ShowName);
		}

		UP13LobbyLevelSelectWidget* LevelButton = CreateWidget<UP13LobbyLevelSelectWidget>(LobbyController, LevelButtonWidgetClass);
		check(LevelButton)

		LevelButton->InitLobbyLevelButton(LevelRows[Index], this);
		LevelButton->AddToViewport();
		LevelsHorizontalBox->AddChild(LevelButton);
	}
}

void UP13LobbyMenuWidget::ClearLevels() const
{
	LevelsHorizontalBox->ClearChildren();
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Level Select                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13LobbyLevelSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP13LobbyLevelSelectWidget::OnLevelButtonPressed()
{
	CachedLobbyMenu->SelectLevelName(LevelName->GetText(), GetLevelAddress());
}

void UP13LobbyLevelSelectWidget::InitLobbyLevelButton(const FP13LevelSelect* NewLevel, const UP13LobbyMenuWidget* LobbyMenu)
{
	InitLevelButton(NewLevel);
	CachedLobbyMenu = LobbyMenu;
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
