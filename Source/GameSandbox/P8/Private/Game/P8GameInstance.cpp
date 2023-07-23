// Copyright (C) 2023, IKinder

#include "P8/Public/Game/P8GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(LogP8GameInstance, All, All);

UP8GameInstance::UP8GameInstance(const FObjectInitializer& ObjectInitializer)
{
	const ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBPClass(TEXT("/Game/Project/P8/Widget/WBP_MainMenu"));
	if (MainMenuBPClass.Class)
	{
		MainMenuClass = MainMenuBPClass.Class;
		UE_LOG(LogP8GameInstance, Log, TEXT("Found class %s"), *MainMenuClass->GetName());
	}
}

void UP8GameInstance::Init()
{
	Super::Init();
	UE_LOG(LogP8GameInstance, Log, TEXT("Game instance initialized"));
}

void UP8GameInstance::Host()
{
	HostServer();
}

void UP8GameInstance::Join(const FString& Address)
{
	JoinServer(Address);
}

void UP8GameInstance::LoadMenu() const
{
	if (!MainMenuClass) return;
	UUserWidget* MainMenuWidget =  CreateWidget<UUserWidget>(GetWorld(), MainMenuClass);
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
