// Copyright (C) 2023, IKinder

#include "P12/Public/Game/P12GameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemTypes.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "Online/OnlineSessionNames.h"

UP12GameInstance::UP12GameInstance()
{
	OnCreateSessionCompleteDelegate = FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete);
	OnStartSessionCompleteDelegate = FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartOnlineGameComplete);
	OnFindSessionsCompleteDelegate = FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionComplete);
	OnJoinSessionCompleteDelegate = FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete);
	OnDestroySessionCompleteDelegate = FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete);
}

void UP12GameInstance::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, ServerName);
	DOREPLIFETIME(ThisClass, MaxPlayers);
}

void UP12GameInstance::Init()
{
	Super::Init();

	OnNetworkFailureEventHandle = GEngine->NetworkFailureEvent.AddUObject(this, &ThisClass::OnNetworkFailure);
	OnTravelFailureEventHandle = GEngine->TravelFailureEvent.AddUObject(this, &ThisClass::OnTravelFailure);
}

void UP12GameInstance::Shutdown()
{
	GEngine->NetworkFailureEvent.Remove(OnNetworkFailureEventHandle);
	GEngine->TravelFailureEvent.Remove(OnTravelFailureEventHandle);
	
	Super::Shutdown();
}

bool UP12GameInstance::ProcessConsoleExec(const TCHAR* Cmd, FOutputDevice& Ar, UObject* Executor)
{
	bool bResult = Super::ProcessConsoleExec(Cmd, Ar, Executor);
	if (!bResult)
	{
		TArray<UGameInstanceSubsystem*> Subsystems = GetSubsystemArray<UGameInstanceSubsystem>();
		for (UGameInstanceSubsystem* Subsystem : Subsystems)
		{
			bResult |= Subsystem->ProcessConsoleExec(Cmd, Ar, Executor);
		}
	}
	
	return bResult;
}

void UP12GameInstance::DisplayNetworkErrorMessage(const FString& ErrorMessage)
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, *ErrorMessage);
}

IOnlineSessionPtr UP12GameInstance::GetSessionsFromOnlineSubsystem()
{
	/* Get online subsystem, so we can get session interface. */
	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		return nullptr;
	}
	return OnlineSubsystem->GetSessionInterface();
}

/*------ CreatingNetworkSession ------*/

bool UP12GameInstance::HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bLAN, bool bPresence, int32 MaxNumPlayers)
{	
	/* Get session interface, so we can call "CreateSession" method on it. */
	const IOnlineSessionPtr Sessions = GetSessionsFromOnlineSubsystem();
	if (!Sessions.IsValid() || !UserId.IsValid())
	{
		return false;
	}
	/**
	 * Fill in all session settings we want to use.
	 * There are more with SessionSettings.Set(...);
	 * For example the Map or the GameMode/Type.
	 */
	SessionSettings = MakeShareable(new FOnlineSessionSettings());
	SessionSettings->bIsLANMatch = bLAN;
	SessionSettings->bUsesPresence = bPresence;
	SessionSettings->NumPublicConnections = MaxNumPlayers;
	SessionSettings->NumPrivateConnections = 0;
	SessionSettings->bAllowInvites = true;
	SessionSettings->bAllowJoinInProgress = true;
	SessionSettings->bShouldAdvertise = true;
	SessionSettings->bAllowJoinViaPresence = true;
	SessionSettings->bAllowJoinViaPresenceFriendsOnly = false;

	SessionSettings->Set(SETTING_MAPNAME, LobbyMapName.ToString(), EOnlineDataAdvertisementType::ViaOnlineService);

	/* Set the delegate to the handle of the session interface. */
	OnCreateSessionCompleteDelegateHandle = Sessions->AddOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegate);
	/* ::OnCreateSessionComplete(...) */
	
	/* Our delegate should get called when this is complete (does not need to be successful). */
	return Sessions->CreateSession(*UserId, SessionName, *SessionSettings);
}

void UP12GameInstance::OnCreateSessionComplete(FName SessionName, bool bSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("On create session complete %s, %d"), *SessionName.ToString(), bSuccessful));
	if (!bSuccessful)
	{
		DisplayNetworkErrorMessage("Failed to create session, please try again");
		return;
	}
	const IOnlineSessionPtr Sessions = GetSessionsFromOnlineSubsystem();
	if (!Sessions.IsValid())
	{
		return;
	}
	/* Clear the session delegate handle, since we finished this call. */
	Sessions->ClearOnCreateSessionCompleteDelegate_Handle(OnCreateSessionCompleteDelegateHandle);
	/* Set the start session delegate handle. */
	OnStartSessionCompleteDelegateHandle = Sessions->AddOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegate);
	/* ::OnStartOnlineGameComplete(...) */

	/* Our StartSessionComplete delegate should get called after this. */
	Sessions->StartSession(SessionName);
}

void UP12GameInstance::OnStartOnlineGameComplete(FName SessionName, bool bSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnStartSessionComplete %s, %d"), *SessionName.ToString(), bSuccessful));
	if (!bSuccessful)
	{
		DisplayNetworkErrorMessage(TEXT("OnStartSessionCompletCannot start online game! Please try again"));
		return;
	}
	const IOnlineSessionPtr Sessions = GetSessionsFromOnlineSubsystem();
	if (!Sessions.IsValid())
	{
		return;
	}
	/* Clear the delegate, since we are done with this call. */
	Sessions->ClearOnStartSessionCompleteDelegate_Handle(OnStartSessionCompleteDelegateHandle);

	/* If the start was successful, we can open a NewMap if we want. Make sure to use "listen" as a parameter. */
	UGameplayStatics::OpenLevel(GetWorld(), LobbyMapName, true, "listen");
}

/*------ FindingNetworkSession ------*/

void UP12GameInstance::FindSession(TSharedPtr<const FUniqueNetId> UserId, bool bLAN, bool bPresence)
{
	const IOnlineSessionPtr Sessions = GetSessionsFromOnlineSubsystem();
	if (!Sessions.IsValid() || !UserId.IsValid())
	{
		/* If something goes wrong, just call the Delegate Function directly with "false". */
		OnFindSessionComplete(false);
		return;
	}
	
	/* Fill in all SearchSettings, like if we are searching for a LAN game and how many results we want to have. */
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	SessionSearch->bIsLanQuery = bLAN;
	SessionSearch->MaxSearchResults = 20;
	SessionSearch->PingBucketSize = 50;

	/* We want to set this query settings if bPresence is true */
	if (bPresence)
	{
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, bPresence, EOnlineComparisonOp::Equals);
	}
	/* Set the delegate to delegate handle of the find session function. */
	OnFindSessionsCompleteDelegateHandle = Sessions->AddOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegate);
	/* ::OnFindSessionComplete(...) */
	
	/* Find session */
	Sessions->FindSessions(*UserId, SessionSearch.ToSharedRef());
}

void UP12GameInstance::OnFindSessionComplete(const bool bSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OFindSessionsComplete bSuccess: %d"), bSuccessful));

	bool bMatchFound = false;
	const IOnlineSessionPtr Sessions = GetSessionsFromOnlineSubsystem();
	if (!Sessions.IsValid())
	{
		OnMatchFound.Broadcast(bMatchFound);
		return;
	}
	
	/* Clear the delegate handle, since we finished this call. */
	Sessions->ClearOnFindSessionsCompleteDelegate_Handle(OnFindSessionsCompleteDelegateHandle);
	/* Just debugging the Number of Search results. Can be displayed in UMG or something later on. */
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Num Search Results: %d"), SessionSearch->SearchResults.Num()));
	/* If we found at least one session, we just going to debug them. You could add them to a list of UMG widget, like it is done in the BP version. */
	TArray<FOnlineSessionSearchResult> SearchResults = SessionSearch->SearchResults;
	if (SearchResults.IsEmpty())
	{
		OnMatchFound.Broadcast(bMatchFound);
		return;
	}
	
	/* "SessionSearch->SearchResults" is an Array that contains all the information.
	 * You can access the Session in this and get a lot of information.
	 * This can be customized later on with your own classes to add more information that can be set and displayed. */
	for (int32 SearchIndex = 0; SearchIndex < SearchResults.Num(); ++SearchIndex)
	{
		/* OwningUserName is just the SessionName for now.
		 * I guess you can create your own Host Settings class and GameSession Class and add a proper GameServer Name here.
		 * This can be customized later on with your own classes to add more information that can be set and displayed. */
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("Session Number: %d | Sessionname: %s "), SearchIndex + 1, *(SearchResults[SearchIndex].Session.OwningUserName)));
	}
	bMatchFound = true;
	OnMatchFound.Broadcast(bMatchFound);
}

/*------ JoiningNetworkSession ------*/

bool UP12GameInstance::JoinFoundOnlineSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult)
{
	/* Return bool */
	bool bSuccessful = false;
	
	const IOnlineSessionPtr Sessions = GetSessionsFromOnlineSubsystem();
	if (!Sessions.IsValid() || !UserId.IsValid())
	{
		return bSuccessful;
	}
	/* Set the Handle again. */
	OnJoinSessionCompleteDelegateHandle = Sessions->AddOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegate);
	/* ::OnJoinSessionComplete(...) */
	
	/* All the "JoinSession" Function with the passed "SearchResult".
	 * The "SessionSearch->SearchResults" can be used to get such a "FOnlineSessionSearchResult" and pass it.
	 * Pretty straight forward! */
	bSuccessful = Sessions->JoinSession(*UserId, SessionName, SearchResult);
	
	return bSuccessful;
}

void UP12GameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	const IOnlineSessionPtr Sessions = GetSessionsFromOnlineSubsystem();
	if (!Sessions.IsValid())
	{
		return;
	}
	/* Clear the Delegate again. */
	Sessions->ClearOnJoinSessionCompleteDelegate_Handle(OnJoinSessionCompleteDelegateHandle);
	/* Get the first local PlayerController, so we can call "ClientTravel" to get to the Server Map.
	 * This is something the Blueprint Node "Join Session" does automatically! */
	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (!PlayerController)
	{
		return;
	}
	/* We need a FString to use ClientTravel and we can let the SessionInterface construct such a
	 * string for us by giving him the SessionName and an empty String.
	 * We want to do this, because every OnlineSubsystem uses different TravelURLs. */
	FString TravelURL;
	if (!Sessions->GetResolvedConnectString(SessionName, TravelURL))
	{
		return;
	}
	PlayerController->ClientTravel(TravelURL, TRAVEL_Absolute);
}

/*------ DestroyingASession ------*/

void UP12GameInstance::OnDestroySessionComplete(FName SessionName, bool bSuccessful)
{
	GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Red, FString::Printf(TEXT("OnDestroySessionComplete %s, %d"), *SessionName.ToString(), bSuccessful));

	const IOnlineSessionPtr Sessions = GetSessionsFromOnlineSubsystem();
	if (!Sessions.IsValid())
	{
		return;
	}
	/* Clear the Delegate. */
	Sessions->ClearOnDestroySessionCompleteDelegate_Handle(OnDestroySessionCompleteDelegateHandle);
	/* If it was successful, we just load another level (could be a MainMenu!). */
	if (!bSuccessful)
	{
		return;
	}
	UGameplayStatics::OpenLevel(GetWorld(), MainMenuMapName);
}

/*------ OnFailure ------*/

void UP12GameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorMessage)
{
	DisplayNetworkErrorMessage(ErrorMessage);
}

void UP12GameInstance::OnTravelFailure(UWorld* World, ETravelFailure::Type FailureType, const FString& ErrorMessage)
{
	DisplayNetworkErrorMessage(ErrorMessage);
}
