// Copyright (C) 2023, IKinder

#include "P11/Public/Player/P11PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "P11/Public/Game/P11GameInstance.h"
#include "P11/Public/Game/P11SavePlayerInfo.h"
#include "P11/Public/Player/P11Character.h"
#include "P11/Public/Player/P11PlayerState.h"
#include "P11/Public/UI/P11HUD.h"

// DEFINE_LOG_CATEGORY_STATIC(LogP11PlayerController, All, All)

void AP11PlayerController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);

	if (AP11PlayerState* ThisPlayerState = GetPlayerState<AP11PlayerState>())
	{
		ThisPlayerState->SetIsAlive();
	}
	if (HasAuthority())
	{
		Client_ChangeSide();
	}
}

void AP11PlayerController::BeginPlay()
{
	Super::BeginPlay();
	SetInputMode(FInputModeGameOnly());

	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultContext, 0);
	}
	HUD = Cast<AP11HUD>(GetHUD());
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
		EnhancedInput->BindAction(ScoreboardAction, ETriggerEvent::Started, this, &ThisClass::ScoreboardInput, true);
		EnhancedInput->BindAction(ScoreboardAction, ETriggerEvent::Completed, this, &ThisClass::ScoreboardInput, false);
	}
}

void AP11PlayerController::MainMenuInput()
{
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

void AP11PlayerController::SetCharSide(const EP11PlayerSide NewSide)
{
	Server_ChangeCharSide(NewSide);
}

void AP11PlayerController::Client_ChangeSide_Implementation()
{
	const UP11GameInstance* GameInstance = Cast<UP11GameInstance>(GetWorld()->GetGameInstance());
	if (!GameInstance)
	{
		return;
	}
	const UP11SavePlayerInfo* SavedInfo = Cast<UP11SavePlayerInfo>(UGameplayStatics::LoadGameFromSlot(GameInstance->GetSlotName(), 0));
	if (!SavedInfo)
	{
		return;
	}
	Server_ChangeCharSide(SavedInfo->PlayerSide);
}

void AP11PlayerController::Server_ChangeCharSide_Implementation(const EP11PlayerSide NewSide)
{
	ChangeCharSide(NewSide);
}

void AP11PlayerController::ChangeCharSide(const EP11PlayerSide NewSide) const
{
	if (AP11Character* Char = Cast<AP11Character>(GetPawn()))
	{
		Char->UpdateMeshOnServer(NewSide);
	}
	
}

void AP11PlayerController::ScoreboardInput(const bool bVisible)
{
	if (!HUD)
	{
		return;
	}
	HUD->ShowScore(bVisible);
}
