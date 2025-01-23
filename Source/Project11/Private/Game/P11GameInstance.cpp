// Copyright (C) 2023, IKinder

#include "Game/P11GameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Game/P11SavePlayerInfo.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "UObject/ConstructorHelpers.h"

void UP11GameInstance::Init()
{
	Super::Init();
	SavePlayerInfo = NewObject<UP11SavePlayerInfo>(GetTransientPackage(), UP11SavePlayerInfo::StaticClass());
	FindSessionInterface();
}

void UP11GameInstance::FindSessionInterface()
{
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return;
	}
	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (SessionInterface)
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionCompleteHandle);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDestroySessionCompleteHandle);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionCompleteHandle);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionCompleteHandle);
	}
	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &ThisClass::OnNetworkFailureHandle);
	}
	CurrentSessionName = GetDefault<APlayerState>()->SessionName;
}

void UP11GameInstance::OnCreateSessionCompleteHandle(FName SessionName, bool bSuccessful) 
{
	if (!bSuccessful || !GetWorld())
	{
		return;
	}
	const FString GoToURL = TargetLevelURL + "?listen";
	GetWorld()->ServerTravel(GoToURL);
}

void UP11GameInstance::OnDestroySessionCompleteHandle(FName SessionName, bool bSuccessful) 
{

}

void UP11GameInstance::OnFindSessionCompleteHandle(bool bSuccessful) 
{
	if (!bSuccessful || !SessionSearch || SessionSearch->SearchResults.IsEmpty())
	{
		OnFindSessionFail.Broadcast();
		return;
	}
	for (const FOnlineSessionSearchResult& Session : SessionSearch->SearchResults)
	{
		if (!SessionSearch.IsValid())
		{
			continue;
		}
		OnFindSessionSuccessful.Broadcast(Session);
	}
}

void UP11GameInstance::OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type Result) 
{

}

void UP11GameInstance::OnNetworkFailureHandle(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type Type, const FString& ErrorString) 
{

}

void UP11GameInstance::Host(const int32 MaxPlayers, const bool bLAN, const FString& LevelURL, const FString& ServerName)
{
	if (!SessionInterface)
	{
		return;
	}
	/* If the session is already exist - destroy it. */
	if (SessionInterface->GetNamedSession(CurrentSessionName))
	{
		SessionInterface->DestroySession(CurrentSessionName);
		/* After async success -> ThisClass::OnDestroySessionCompleteHandle. */
		return;
	}
	CreateSession(MaxPlayers, bLAN, LevelURL, ServerName);
}

void UP11GameInstance::Join(const FOnlineSessionSearchResult& Result)
{
	if (!SessionInterface || !SessionSearch)
	{
		return;
	}
	// SessionInterface->JoinSession(0, CurrentSessionName, SessionSearch->SearchResults[Index]);
	SessionInterface->JoinSession(0, CurrentSessionName, Result);
	/* After async success -> ThisClass::OnJoinSessionCompleteHandle. */
}

void UP11GameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch)
	{
		SessionSearch->MaxSearchResults = 20;
		SessionSearch->QuerySettings.Set(TEXT("PRESENCESEARCH"), true, EOnlineComparisonOp::Equals);
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		/* After async success -> ThisClass::OnFindSessionCompleteHandle. */
	}
}

void UP11GameInstance::CreateSession(const int32 MaxPlayers, const bool bLAN, const FString& LevelURL, const FString& ServerName)
{
	if (!SessionInterface)
	{
		return;
	}
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = bLAN;
	SessionSettings.NumPublicConnections = MaxPlayers;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	/* Add server name via custom field in session settings. */
	SessionSettings.Set(ServerNameKey, ServerName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);
	TargetLevelURL = LevelURL;
	
	SessionInterface->CreateSession(0, CurrentSessionName, SessionSettings);
	/* After async success -> ThisClass::OnCreateSessionCompleteHandle. */
}