// Copyright © 2024, IKinder

#include "Controller/P13MenuPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "UI/Menu/P13LobbyWidget.h"

AP13MenuPlayerController::AP13MenuPlayerController() {}

void AP13MenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenu();
	SetMenuInputMode();
}

void AP13MenuPlayerController::ShowMainMenu()
{
	check(MainMenuWidgetClass)

	if (!IsLocalController())
	{
		return;
	}

	UP13StartMenuWidget* MainMenuWidget = CreateWidget<UP13StartMenuWidget>(this, MainMenuWidgetClass);
	if (!MainMenuWidget)
	{
		return;
	}

	MainMenuWidget->AddToViewport();
}

void AP13MenuPlayerController::SetMenuInputMode() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	FInputModeUIOnly UIMode;
	UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

	PlayerController->SetInputMode(UIMode);
	PlayerController->bShowMouseCursor = true;
}

void AP13MenuPlayerController::SetGameInputMode() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	FInputModeGameOnly GIMode;
	GIMode.SetConsumeCaptureMouseDown(false);
	PlayerController->SetInputMode(GIMode);
	PlayerController->bShowMouseCursor = false;
}
