// Copyright (C) 2023, IKinder

#include "P11/Public/UI/ServerList/P11ServerList.h"

#include "OnlineSessionSettings.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "Components/WidgetSwitcher.h"
#include "P11/Public/Game/P11GameInstance.h"
#include "P11/Public/UI/ServerList/P11ServerInfo.h"

void UP11ServerList::NativeConstruct()
{
	Super::NativeConstruct();
	GameInstance = GetWorld()->GetGameInstance<UP11GameInstance>();
	check(GameInstance)
	GameInstance->OnFindSessionFail.AddUObject(this, &ThisClass::OnFindSessionFailHandle);
	GameInstance->OnFindSessionSuccessful.AddUObject(this, &ThisClass::OnFindSessionSuccessfulHandle);
	GameInstance->RefreshServerList();

	if (!BackBtn || !RefreshBtn || !ServerListScrl)
	{
		return;
	}
	BackBtn->OnClicked.AddDynamic(this, &ThisClass::OnBackHandle);
	RefreshBtn->OnClicked.AddDynamic(this, &ThisClass::OnRefreshHandle);
}

void UP11ServerList::OnBackHandle()
{
	OnBackServerListBackPressed.Broadcast();
}

void UP11ServerList::OnRefreshHandle()
{
	ListSwitcher->SetActiveWidgetIndex(SERVER_LIST_LOADING_INDEX);
	ServerListScrl->ClearChildren();
	GameInstance->RefreshServerList();
}

void UP11ServerList::OnFindSessionFailHandle()
{
	ListSwitcher->SetActiveWidgetIndex(SERVER_LIST_FAILURE_INDEX);
}

void UP11ServerList::OnFindSessionSuccessfulHandle(const FOnlineSessionSearchResult& SessionSearchResult)
{
	ListSwitcher->SetActiveWidgetIndex(SERVER_LIST_SUCCESS_INDEX);
	UP11ServerInfo* ServerInfoWidget = CreateWidget<UP11ServerInfo>(this, ServerInfoClass);
	if (!ServerInfoWidget)
	{
		return;
	}
	ServerInfoWidget->SetSessionInfo(SessionSearchResult);
	ServerListScrl->AddChild(ServerInfoWidget);
}
