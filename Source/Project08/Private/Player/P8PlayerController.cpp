// Copyright (C) 2023, IKinder

#include "Player/P8PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Game/P8GameInstance.h"
#include "Game/P8MainGameHUD.h"
#include "Widget/P8InGameMenuWidget.h"

AP8PlayerController::AP8PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP8PlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultContext, 0);
	}
}

void AP8PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (InputComponent)
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
		{
			EnhancedInputComponent->BindAction(GamePauseAction, ETriggerEvent::Started, this, &ThisClass::SwitchGamePause);
		}
	}
}

void AP8PlayerController::UnPause()
{
	if (!bGamePause) return;
	SwitchGamePause();
}

void AP8PlayerController::GoToLobby()
{
	const UP8GameInstance* GameInstance = Cast<UP8GameInstance>(GetGameInstance());
	if (!GameInstance) return;
	ClientTravel(GameInstance->GetStartURL(), TRAVEL_Absolute);
}

void AP8PlayerController::SwitchGamePause()
{
	/* Flip flop Game Pause state and set game to pause depends on state. Cursor will be visible when game is paused. */
	bGamePause = !bGamePause;
	SetPause(bGamePause);
	bShowMouseCursor = bGamePause;
	/* Set input mode. */
	FInputModeGameAndUI PauseInputMode;
	PauseInputMode.SetHideCursorDuringCapture(false);
	bGamePause ? SetInputMode(PauseInputMode) : SetInputMode(FInputModeGameOnly());
	/* Show pause widget if game is paused. */
	AP8MainGameHUD* MainGameHUD = Cast<AP8MainGameHUD>(GetHUD());
	if (!MainGameHUD) return;
	GamePauseWidget = MainGameHUD->ShowPauseMenu(bGamePause);
	/* Send reference of player controller interface to pause widget.
	 * To prevent unnecessary assigns - do it once when PC interface in pause widget is nullptr. */
	if (!GamePauseWidget->GetPCInterface())
	{
		GamePauseWidget->SetPCInterface(this);
	}
}
