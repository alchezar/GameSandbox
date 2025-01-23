// Copyright (C) 2023, IKinder

#include "Game/P8GameInstance.h"

#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "OnlineSubsystemUtils.h"
#include "Blueprint/UserWidget.h"
#include "Game/P8MainMenuHUD.h"
#include "GameFramework/PlayerState.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "Online/OnlineSessionNames.h"
#include "UObject/ConstructorHelpers.h"
#include "Util/P8Utils.h"
#include "Widget/P8MainMenuWidget.h"

DEFINE_LOG_CATEGORY_STATIC(LogP8GameInstance, All, All)

UP8GameInstance::UP8GameInstance(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/Project/P8/Widget/WBP_MainMenu"));
	if (MainMenuBPClass.Class)
	{
		MainMenuClass = MainMenuBPClass.Class;
	}
	const ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/Project/P8/Widget/WBP_ServerRow"));
	if (ServerRowBPClass.Class)
	{
		ServerRowClass = ServerRowBPClass.Class;
	}
}

void UP8GameInstance::Init()
{
	Super::Init();

	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem) return;
	// if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 10.f, FColor::Cyan, FString::Printf(TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString()));

	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (SessionInterface)
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionCompleteHandle);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDeleteSessionCompleteHandle);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionCompleteHandle);
		SessionInterface->OnJoinSessionCompleteDelegates.AddUObject(this, &ThisClass::OnJoinSessionCompleteHandle);
	}
	if (GEngine)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &ThisClass::OnNetworkFailureHandle);
	}
	CurrentSessionName = GetDefault<APlayerState>()->SessionName;
}

void UP8GameInstance::Host()
{
	if (!SessionInterface) return;
	FindMenuWidget();
	
	const FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(CurrentSessionName);
	if (ExistingSession)
	{
		SessionInterface->DestroySession(CurrentSessionName);
		/* OnDeleteSessionCompleteHandle callback when completed. */
		return;
	}
	CreateSession();
}

void UP8GameInstance::Join(uint32 Index)
{
	if (!SessionInterface || !SessionSearch) return;
	SessionInterface->JoinSession(0, CurrentSessionName, SessionSearch->SearchResults[Index]);
	/* OnJoinSessionCompleteHandle callback when completed. */
}

void UP8GameInstance::RefreshServerList()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch)
	{
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(TEXT("PRESENCESEARCH"), true, EOnlineComparisonOp::Equals);
		// SessionSearch->bIsLanQuery = true;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		/* OnFindSessionCompleteHandle callback when completed. */
	}
	/* Once the search is complete, we`ll need a reference to the MainMenu widget. */
	FindMenuWidget();
}

void UP8GameInstance::OnCreateSessionCompleteHandle(FName SessionName, bool bWasSuccessful)
{
	/* Server Host */
	if (!bWasSuccessful || !GetWorld()) return;
	const FString GoToURL = LobbyLevelURL + "?listen";
	if (GetWorld()->ServerTravel(GoToURL))
	{
		if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Hosting: %s"), *SessionName.ToString()));
	}
}

void UP8GameInstance::OnDeleteSessionCompleteHandle(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;
	CreateSession();
}

void UP8GameInstance::OnFindSessionCompleteHandle(bool bWasSuccessful)
{
	if (!bWasSuccessful || !SessionSearch || !MainMenuWidget) return;
	TArray<FP8ServerData> ServerData;
	
	for (const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		if (!SearchResult.IsValid()) continue;
		FP8ServerData Data;

		Data.SessionID = SearchResult.GetSessionIdStr();
		Data.MaxPlayers = SearchResult.Session.SessionSettings.NumPublicConnections;
		Data.CurrentPlayers = Data.MaxPlayers - SearchResult.Session.NumOpenPublicConnections;
		Data.HostUserName = SearchResult.Session.OwningUserName;
		/* Take from our custom settings field, using the same key that was set in CreateSession() */
		SearchResult.Session.SessionSettings.Get(CustomServerNameKey, Data.ServerName);

		ServerData.Add(Data);
	}
	MainMenuWidget->SetServerList(ServerData);
}

void UP8GameInstance::OnJoinSessionCompleteHandle(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!SessionInterface) return;
	FString Address;
	if (!SessionInterface->GetResolvedConnectString(SessionName, Address))
	{
		UE_LOG(LogP8GameInstance, Log, TEXT("Could not ged connecting string."));
		return;
	}
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Joining: %s"), *Address));
	if (!GetWorld()) return;
	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (!PlayerController) return;
	PlayerController->ClientTravel(Address, TRAVEL_Absolute);
}

void UP8GameInstance::OnNetworkFailureHandle(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type Type, const FString& ErrorString)
{
	APlayerController* PlayerController = World->GetFirstPlayerController();
	if (!PlayerController) return;
	PlayerController->ClientTravel(GetStartURL(), TRAVEL_Absolute);
}

void UP8GameInstance::CreateSession()
{
	if (!SessionInterface) return;
	FOnlineSessionSettings SessionSettings;

	SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
	SessionSettings.NumPublicConnections = 3;
	SessionSettings.bShouldAdvertise = true;
	SessionSettings.bUsesPresence = true;
	SessionSettings.bUseLobbiesIfAvailable = true;
	/* Add custom field in session settings by using key. In our case it is CustomServerName */
	SessionSettings.Set(CustomServerNameKey, MainMenuWidget->GetCustomServerName(), EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

	SessionInterface->CreateSession(0, CurrentSessionName, SessionSettings);
	/* OnCreateSessionCompleteHandle callback when completed. */
}

void UP8GameInstance::FindMenuWidget()
{
	if (MainMenuWidget) return;
	const APlayerController* PC = GetFirstLocalPlayerController(GetWorld());
	if (!PC) return;
	const AP8MainMenuHUD* LobbyHUD = Cast<AP8MainMenuHUD>(PC->GetHUD());
	if (!LobbyHUD) return;
	MainMenuWidget = LobbyHUD->GetMainMenuWidget();
}

void UP8GameInstance::StartSession()
{
	if (!SessionInterface) return;
	SessionInterface->StartSession(CurrentSessionName);
}

void UP8GameInstance::LoadMenu() const
{
	if (!MainMenuClass) return;
	UUserWidget* MainMenuWidget_Unused = CreateWidget<UUserWidget>(GetWorld(), MainMenuClass);
	if (!MainMenuWidget_Unused) return;
	MainMenuWidget_Unused->AddToViewport();

	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (!PlayerController) return;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget_Unused->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}
