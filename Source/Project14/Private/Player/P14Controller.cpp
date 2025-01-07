// Copyright © 2025, Ivan Kinder

#include "Player/P14Controller.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

AP14Controller::AP14Controller() {}

void AP14Controller::BeginPlay()
{
	Super::BeginPlay();

	const ULocalPlayer* LocalPlayer = GetLocalPlayer();
	check(LocalPlayer)

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer))
	{
		Subsystem->AddMappingContext(InputContext, 0);
	}
}

void AP14Controller::SetupInputComponent()
{
	Super::SetupInputComponent();

	check(InputComponent)
	if (UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(InputComponent))
	{
		Input->BindActionValue(MenuAction);

		Input->BindAction(MenuAction, ETriggerEvent::Started, this, &ThisClass::TogglePause);
	}
}

void AP14Controller::TogglePause()
{
	// Toggle game pause.
	const bool bPaused = !IsPaused();
	SetPause(bPaused);
	bShowMouseCursor = bPaused;
	bPaused ? SetInputMode(FInputModeGameAndUI{}.SetHideCursorDuringCapture(false)) : SetInputMode(FInputModeGameOnly{});
}
