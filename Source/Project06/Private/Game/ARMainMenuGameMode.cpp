// Copyright (C) 2023, IKinder

#include "Game/ARMainMenuGameMode.h"

#include "Blueprint/UserWidget.h"
#include "UI/Menu/ARMainMenu.h" // ReSharper disable once CppUnusedIncludeDirective

AARMainMenuGameMode::AARMainMenuGameMode()
{
	DefaultPawnClass = nullptr;
}

void AARMainMenuGameMode::BeginPlay()
{
	Super::BeginPlay();

	UARMainMenu* MainMenuWidget = CreateWidget<UARMainMenu>(GetWorld(), MainMenuWidgetClass);
	if (!MainMenuWidget) return;
	
	MainMenuWidget->AddToViewport();	
	APlayerController* Controller = GetWorld()->GetFirstPlayerController();
	if (!Controller) return;
	
	Controller->bShowMouseCursor = true;	
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	Controller->SetInputMode(InputMode);
}
