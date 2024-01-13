// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameModeMenu.h"

#include "P13/Public/UI/P13MenuWidget.h"

class UP13MenuWidget;

void AP13GameModeMenu::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenu();
	SetMenuInputMode();
}

void AP13GameModeMenu::ShowMainMenu()
{
	check(MainMenuWidgetClass)

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	MainMenuWidget = CreateWidget<UP13MenuWidget>(PlayerController, MainMenuWidgetClass);
	check(MainMenuWidget)

	MainMenuWidget->AddToViewport();
}

void AP13GameModeMenu::SetMenuInputMode()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	FInputModeUIOnly UIMode;
	UIMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

	PlayerController->SetInputMode(UIMode);
	PlayerController->bShowMouseCursor = true;
}

void AP13GameModeMenu::SetGameInputMode()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	FInputModeGameOnly GIMode;
	GIMode.SetConsumeCaptureMouseDown(false);
	PlayerController->SetInputMode(GIMode);
	PlayerController->bShowMouseCursor = false;
}
