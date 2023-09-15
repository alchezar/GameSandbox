// Copyright (C) 2023, IKinder

#include "P11/Public/Player/P11PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "P11/Public/UI/P11HUD.h"

void AP11PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
		
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultContext, 0);
	}
}

void AP11PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent)
	{
		return;
	}
	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(MainMenuAction, ETriggerEvent::Started, this, &ThisClass::MainMenuInput);
	}
}

void AP11PlayerController::MainMenuInput()
{
	AP11HUD* HUD = Cast<AP11HUD>(GetHUD());
	if (!HUD)
	{
		return;
	}
	bMenuVisibility = !bMenuVisibility;
	// SetPause(bMenuVisibility);
	HUD->ShowMainMenu(bMenuVisibility);
	SetShowMouseCursor(bMenuVisibility);
	
	FInputModeGameAndUI GameAndUIMode;
	GameAndUIMode.SetHideCursorDuringCapture(false);
	bMenuVisibility ? SetInputMode(GameAndUIMode) : SetInputMode(FInputModeGameOnly());
	bMenuVisibility ? GetPawn()->DisableInput(this) : GetPawn()->EnableInput(this); 
}
