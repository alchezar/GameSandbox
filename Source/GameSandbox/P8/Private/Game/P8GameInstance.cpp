// Copyright (C) 2023, IKinder

#include "P8/Public/Game/P8GameInstance.h"
#include "OnlineSessionSettings.h"
#include "OnlineSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogP8GameInstance, All, All);

UP8GameInstance::UP8GameInstance(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/Project/P8/Widget/WBP_MainMenu"));
	if (MainMenuBPClass.Class)
	{
		MainMenuClass = MainMenuBPClass.Class;
		// UE_LOG(LogP8GameInstance, Log, TEXT("Found class %s"), *MainMenuClass->GetName());
	}
}

void UP8GameInstance::Init()
{
	Super::Init();

	const IOnlineSubsystem* OnlineSubsystem = IOnlineSubsystem::Get();
	if (!OnlineSubsystem)
	{
		UE_LOG(LogP8GameInstance, Log, TEXT("Found no subsystem"));
		return;
	}
	UE_LOG(LogP8GameInstance, Log, TEXT("Found subsystem %s"), *OnlineSubsystem->GetSubsystemName().ToString());

	SessionInterface = OnlineSubsystem->GetSessionInterface();
	if (SessionInterface)
	{
		SessionInterface->OnCreateSessionCompleteDelegates.AddUObject(this, &ThisClass::OnCreateSessionCompleteHandle);
		SessionInterface->OnDestroySessionCompleteDelegates.AddUObject(this, &ThisClass::OnDeleteSessionCompleteHandle);
		SessionInterface->OnFindSessionsCompleteDelegates.AddUObject(this, &ThisClass::OnFindSessionCompleteHandle);
	}

	SessionSearch = MakeShareable(new FOnlineSessionSearch());
	if (SessionSearch)
	{
		// SessionSearch->bIsLanQuery = true;
		SessionInterface->FindSessions(0, SessionSearch.ToSharedRef());
		UE_LOG(LogP8GameInstance, Log, TEXT("Starting to find session"));
	}
}

void UP8GameInstance::Host()
{
	if (!SessionInterface) return;
	const FNamedOnlineSession* ExistingSession = SessionInterface->GetNamedSession(CurrentSessionName);
	if (ExistingSession)
	{
		SessionInterface->DestroySession(CurrentSessionName);
		return;
	}
	CreateSession();
}

void UP8GameInstance::Join(const FString& Address)
{
	JoinServer(Address);
}

void UP8GameInstance::LoadMenu() const
{
	if (!MainMenuClass) return;
	UUserWidget* MainMenuWidget = CreateWidget<UUserWidget>(GetWorld(), MainMenuClass);
	if (!MainMenuWidget) return;
	MainMenuWidget->AddToViewport();

	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (!PlayerController) return;
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}

void UP8GameInstance::HostServer() const
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, "Hosting");
	if (!GetWorld()) return;
	const FString GoToURL = GameLevelURL + "?listen";
	GetWorld()->ServerTravel(GoToURL);
}

void UP8GameInstance::JoinServer(const FString& Address) const
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 2.f, FColor::Cyan, FString::Printf(TEXT("Joining: %s"), *Address));
	if (!GetWorld()) return;
	APlayerController* PlayerController = GetFirstLocalPlayerController(GetWorld());
	if (!PlayerController) return;
	PlayerController->ClientTravel(Address, TRAVEL_Absolute);
}

void UP8GameInstance::OnCreateSessionCompleteHandle(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;
	HostServer();
}

void UP8GameInstance::OnDeleteSessionCompleteHandle(FName SessionName, bool bWasSuccessful)
{
	if (!bWasSuccessful) return;
	CreateSession();
}

void UP8GameInstance::OnFindSessionCompleteHandle(bool bWasSuccessful)
{
	if (!SessionSearch) return;
	
	UE_LOG(LogP8GameInstance, Log, TEXT("Session found"));
	for(const FOnlineSessionSearchResult& SearchResult : SessionSearch->SearchResults)
	{
		if (!SearchResult.IsValid()) continue;
		UE_LOG(LogP8GameInstance, Log, TEXT("Found session name: %s"), *SearchResult.GetSessionIdStr());
	}
}

void UP8GameInstance::CreateSession()
{
	if (!SessionInterface) return;
	FOnlineSessionSettings SessionSettings;
	SessionSettings.bIsLANMatch = true;
	SessionSettings.NumPublicConnections = 2;
	SessionSettings.bShouldAdvertise = true;
	SessionInterface->CreateSession(0, CurrentSessionName, SessionSettings);
}

// Tests
void UP8GameInstance::Test1() {}

void UP8GameInstance::Test2() {}
