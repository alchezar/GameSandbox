// Copyright © 2024, IKinder

#include "P13/Public/Controller/P13LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "P13/Public/UI/Menu/P13LobbyWidget.h"

AP13LobbyPlayerController::AP13LobbyPlayerController()
{
	bReplicates = true;
}

void AP13LobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, CurrentLevelByServer)
}

void AP13LobbyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	ShowLobbyMenu();
	SetMenuInputMode();
	ListenToGameMode();
}

void AP13LobbyPlayerController::Server_UpdateClientReady_Implementation()
{
	
}

void AP13LobbyPlayerController::OnHostSelectedMap(const FText& SelectedLevelName, const FName SelectedLevelAddress)
{
	/* We don`t need to mark this method "Server_", anyway it will be called only by the host.
	 * Because Clients won't see Levels list. */
	
	CachedLobbyGameMode->UpdateSelectedLevel(SelectedLevelName, SelectedLevelAddress);
}

void AP13LobbyPlayerController::UpdateSelectedMapName(const FText& SelectedLevelName) const
{
	if (!LobbyMenuWidget)
	{
		return;
	}
	LobbyMenuWidget->UpdateLevelName(SelectedLevelName);
}

void AP13LobbyPlayerController::ShowLobbyMenu()
{
	check(LobbyMenuWidgetClass)

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	LobbyMenuWidget = CreateWidget<UP13LobbyMenuWidget>(PlayerController, LobbyMenuWidgetClass);
	check(LobbyMenuWidget)

	LobbyMenuWidget->AddToViewport();
}

void AP13LobbyPlayerController::SetMenuInputMode()
{
	FInputModeUIOnly UIMode;
	UIMode.SetWidgetToFocus(LobbyMenuWidget->TakeWidget());
	UIMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);

	SetInputMode(UIMode);
	bShowMouseCursor = true;
}

void AP13LobbyPlayerController::ListenToGameMode()
{
	check(GetWorld()->GetAuthGameMode() && GetWorld()->GetAuthGameMode()->IsA<AP13LobbyGameMode>())
	CachedLobbyGameMode = GetWorld()->GetAuthGameMode<AP13LobbyGameMode>();
}
