// Copyright (C) 2023, IKinder

#include "UI/ServerList/P11ServerInfo.h"

#include "OnlineSessionSettings.h"
#include "Components/TextBlock.h"
#include "Game/P11GameInstance.h"

void UP11ServerInfo::NativeConstruct()
{
	Super::NativeConstruct();
	GameInstance = GetWorld()->GetGameInstance<UP11GameInstance>();
	check(GameInstance)
}

void UP11ServerInfo::SetSessionInfo(const FOnlineSessionSearchResult& SessionSearchResult)
{
	FString CurrentServerName;
	SessionSearchResult.Session.SessionSettings.Get(GameInstance->GetServerNameKey(), CurrentServerName);
	const int32 Ping = SessionSearchResult.PingInMs;
	const int32 MaxPlayers = SessionSearchResult.Session.SessionSettings.NumPublicConnections;
	const int32 CurrentPlayers = MaxPlayers - SessionSearchResult.Session.NumOpenPublicConnections;

	ServerNameText->SetText(FText::FromString(CurrentServerName));
	PlayersText->SetText(FText::FromString(CurrentPlayers + " / " + MaxPlayers));
	PingText->SetText(FText::AsNumber(Ping));
	Session = SessionSearchResult;
}

FReply UP11ServerInfo::NativeOnMouseButtonDoubleClick(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent)
{
	GameInstance->Join(Session);

	return Super::NativeOnMouseButtonDoubleClick(InGeometry, InMouseEvent);
}
