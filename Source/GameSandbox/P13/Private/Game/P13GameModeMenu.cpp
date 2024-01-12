// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameModeMenu.h"

#include "P13/Public/UI/MainMenu/P13MainMenuWidget.h"

void AP13GameModeMenu::BeginPlay()
{
	Super::BeginPlay();
	ShowMainMenu();
	
}

void AP13GameModeMenu::ShowMainMenu()
{
	check(MainMenuWidgetClass)

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	MainMenuWidget = CreateWidget<UP13MainMenuWidget>(PlayerController, MainMenuWidgetClass);
	check(MainMenuWidget)

	MainMenuWidget->AddToViewport();
}

void AP13GameModeMenu::SetMenuInputMode()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	FInputModeGameAndUI InputMode;
	InputMode.SetHideCursorDuringCapture(false);
	InputMode.SetWidgetToFocus(MainMenuWidget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	PlayerController->SetInputMode(InputMode);
	PlayerController->bShowMouseCursor = true;
}
