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

	OnLogin();
}

void AP13LobbyPlayerController::Server_UpdateClientReady_Implementation()
{
	
}

void AP13LobbyPlayerController::OnHostSelectedMap(const FText& SelectedLevelName) const
{
	/* We don`t need to mark this method "Server_", anyway it will be called only by the host.
	 * Because Clients won't see Levels list. */
	
	CachedLobbyGameMode->UpdateSelectedLevelForAll(SelectedLevelName.ToString());
}

void AP13LobbyPlayerController::UpdateSelectedMapName(const FText& SelectedLevelName) const
{
	if (!LobbyMenuWidget)
	{
		return;
	}
	LobbyMenuWidget->UpdateLevelName(SelectedLevelName);
}

void AP13LobbyPlayerController::OnLogin()
{
	ShowLobbyMenu();
	SetMenuInputMode();
	ListenToGameMode();
}

void AP13LobbyPlayerController::Server_OnPlayerColorSelected_Implementation(const FLinearColor LinearColor)
{
	if (!GetWorld()->GetAuthGameMode() || !CachedLobbyGameMode.Get())
	{
		return;
	}
	CachedLobbyGameMode->UpdateSelectedColor(LinearColor, this);	
}

void AP13LobbyPlayerController::UpdateSelectedColorOccupation(const FLinearColor SelectedColor, const AP13LobbyPlayerController* Occupier) const
{
	if (!LobbyMenuWidget)
	{
		return;
	}
	if (this == Occupier)
	{
		LobbyMenuWidget->ReleaseOccupiedColor();
	}
	LobbyMenuWidget->OccupyColor(SelectedColor);
}

bool AP13LobbyPlayerController::ShowLobbyMenu()
{
	if (LobbyMenuWidget)
	{
		return false;
	}
	check(LobbyMenuWidgetClass)

	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	check(PlayerController);

	LobbyMenuWidget = CreateWidget<UP13LobbyMenuWidget>(PlayerController, LobbyMenuWidgetClass);
	check(LobbyMenuWidget)

	LobbyMenuWidget->AddToViewport();
	return true;
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
