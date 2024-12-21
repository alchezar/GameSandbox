// Copyright (C) 2023, IKinder

#include "Player/ARPlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Blueprint/UserWidget.h"

AARPlayerController::AARPlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARPlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());
	AddDefaultMappingContext();
}

void AARPlayerController::BeginPlayingState()
{
	Super::BeginPlayingState();
	AddWidget();
}

void AARPlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AARPlayerController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);
	OnPawnChanged.Broadcast(InPawn);
}

void AARPlayerController::AddWidget()
{
	if (!HUDClass) return;	
	HUDWidget = CreateWidget(GetWorld(), HUDClass);
	HUDWidget->AddToViewport(1);

	if (!PauseMenuClass) return;
	PauseMenuInstance = CreateWidget(GetWorld(), PauseMenuClass);
	PauseMenuInstance->AddToViewport(2);
	PauseMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
}

void AARPlayerController::AddDefaultMappingContext()
{
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AARPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	auto* EnhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent);
	if (!EnhancedInputComponent) return;

	EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Completed, this, &ThisClass::TogglePauseMenu);	
}

void AARPlayerController::TogglePauseMenu()
{
	bPause = !bPause;
	SetPause(bPause);
	bShowMouseCursor = bPause;
	PauseMenuInstance->SetVisibility(bPause ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
	if (bPause)
	{
		FInputModeGameAndUI InputMode;
		InputMode.SetHideCursorDuringCapture(false);
		SetInputMode(InputMode);
		PauseMenuInstance->SetVisibility(ESlateVisibility::Visible);
		return;
	}
	SetInputMode(FInputModeGameOnly());
	PauseMenuInstance->SetVisibility(ESlateVisibility::Collapsed);
}
