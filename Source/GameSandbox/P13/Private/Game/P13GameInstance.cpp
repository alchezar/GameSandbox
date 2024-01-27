// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "GameFramework/PlayerState.h"
#include "Kismet/GameplayStatics.h"
#include "Online/OnlineSessionNames.h"

void UP13GameInstance::Init()
{
	Super::Init();
	FindSessionInterface();
}

void UP13GameInstance::HostSession(const int32 MaxPlayers, const bool bLan, const FString& CustomServerName)
{
	if (!SessionInterface)
	{
		return;
	}
	if (SessionInterface->GetNamedSession(CurrentSessionName))
	{
		SessionInterface->DestroySession(CurrentSessionName);
	}

	FOnlineSessionSettings SessionSettings;
	SessionSettings.NumPublicConnections = MaxPlayers;
	SessionSettings.bIsLANMatch = bLan;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.Set(SERVER_NAME_KEY , CustomServerName,EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	
	SessionInterface->CreateSession(0, CurrentSessionName, SessionSettings);
}

void UP13GameInstance::FindSessions(const bool bLan)
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch.IsValid())
	{
		SessionSearch->bIsLanQuery = bLan;
		SessionSearch->MaxSearchResults = 20;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UP13GameInstance::JoinSession(const FOnlineSessionSearchResult& Result)
{
	if (!SessionInterface || !SessionSearch)
	{
		return;
	}
	SessionInterface->JoinSession(0, CurrentSessionName, Result);
}

FP13WeaponInfo* UP13GameInstance::GetWeaponInfoByID(const FName WeaponID) const
{
	if (!WeaponInfoTable)
	{
		return nullptr;
	}
	return WeaponInfoTable->FindRow<FP13WeaponInfo>(WeaponID, nullptr);
}

FP13WeaponDrop* UP13GameInstance::GetWeaponDropByID(const FName WeaponID) const
{
	if (!WeaponDropTable)
	{
		return nullptr;
	}
	return WeaponDropTable->FindRow<FP13WeaponDrop>(WeaponID, nullptr);
}

void UP13GameInstance::FindSessionInterface()
{
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return;
	}
	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (!SessionInterface.IsValid())
	{
		return;
	}

	SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionCompleteHandle);
	SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionCompleteHandle);
	SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionCompleteHandle);
	
	CurrentSessionName = GetDefault<APlayerState>()->SessionName;
}

void UP13GameInstance::OnCreateSessionCompleteHandle(FName SessionName, bool bSuccess)
{
	UGameplayStatics::OpenLevel(GetWorld(), LobbyLevelURL, true, "listen");
}

void UP13GameInstance::OnFindSessionCompleteHandle(bool bSuccess)
{
	if (!bSuccess || !SessionSearch.IsValid() || !SessionSearch->SearchResults.IsEmpty())
	{
		return;
	}
	
	OnFindSessionsComplete.Broadcast(SessionSearch->SearchResults);
}

void UP13GameInstance::OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	UGameplayStatics::OpenLevel(GetWorld(), SessionName);	
}
