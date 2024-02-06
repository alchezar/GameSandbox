// Copyright © 2024, IKinder

#include "P13/Public/Controller/P13LobbyPlayerController.h"

#include "Blueprint/UserWidget.h"
#include "Engine/TargetPoint.h"
#include "Net/UnrealNetwork.h"
#include "P13/Public/Character/P13CharacterBase.h"
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

FLinearColor AP13LobbyPlayerController::GetOccupiedColor() const
{
	if (!LobbyMenuWidget)
	{
		return FLinearColor::White;
	}
	return LobbyMenuWidget->GetCurrentOccupiedColor();
}

void AP13LobbyPlayerController::Server_UpdateClientReady_Implementation()
{
	AP13CharacterBase* ControlledPawn = GetPawn<AP13CharacterBase>();
	if (!ControlledPawn)
	{
		return;
	}
	ControlledPawn->PlayReadyAnimation();
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

void AP13LobbyPlayerController::Server_OnPlayerColorSelected_Implementation(const FLinearColor SelectedColor)
{
	if (!GetWorld()->GetAuthGameMode() || !CachedLobbyGameMode.Get())
	{
		return;
	}
	CachedLobbyGameMode->UpdateSelectedColor(SelectedColor, this);	
}

void AP13LobbyPlayerController::Client_UpdateSelectedColorOccupation_Implementation(const FLinearColor SelectedColor, const FLinearColor ReleasedColor, const AP13LobbyPlayerController* Occupier) const
{
	if (!LobbyMenuWidget)
	{
		return;
	}
	LobbyMenuWidget->UpdateReselectedColor(ReleasedColor, SelectedColor);
}

void AP13LobbyPlayerController::OccupyTargetPoint(ATargetPoint* InTargetPoint)
{
	CachedTargetPoint = InTargetPoint;
}

ATargetPoint* AP13LobbyPlayerController::ReleaseTargetPoint()
{
	ATargetPoint* TargetPoint = CachedTargetPoint.Get();
	CachedTargetPoint.Reset();
	return TargetPoint;
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
