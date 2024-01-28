// Copyright © 2024, IKinder

#include "P13/Public/Game/P13LobbyGameMode.h"

#include "P13/Public/UI/Menu/P13LobbyWidget.h"

AP13LobbyGameMode::AP13LobbyGameMode()
{
	bUseSeamlessTravel = true;
}

void AP13LobbyGameMode::BeginPlay()
{
	Super::BeginPlay();

	ShowLobbyMenu();
	// SetMenuInputMode();
}

void AP13LobbyGameMode::ShowLobbyMenu()
{
	check(LobbyMenuWidgetClass)

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	LobbyMenuWidget = CreateWidget<UP13LobbyMenuWidget>(PlayerController, LobbyMenuWidgetClass);
	check(LobbyMenuWidget)

	LobbyMenuWidget->AddToViewport();
}

void AP13LobbyGameMode::SetMenuInputMode() const
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	FInputModeUIOnly UIMode;
	UIMode.SetWidgetToFocus(LobbyMenuWidget->TakeWidget());
	UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

	PlayerController->SetInputMode(UIMode);
	PlayerController->bShowMouseCursor = true;
}
