// Copyright (C) 2023, IKinder

#include "Player/STU_PlayerController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Component/STU_RespawnComponent.h"
#include "Game/STU_GameModeBase.h"
#include "GameFramework/GameModeBase.h"

ASTU_PlayerController::ASTU_PlayerController()
{
	RespawnComponent = CreateDefaultSubobject<USTU_RespawnComponent>("RespawnComponent");
}

void ASTU_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (GetWorld())
	{
		ASTU_GameModeBase* GameMode = Cast<ASTU_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (GameMode)
		{
			GameMode->OnMatchStateChange.AddUObject(this, &ASTU_PlayerController::OnMatchStateChanged);
		}
	}
	
	if (auto* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(this->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}	
}

void ASTU_PlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (!InputComponent) return;

	if (UEnhancedInputComponent* EnhancedInput = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInput->BindAction(PauseAction, ETriggerEvent::Started, this, &ThisClass::OnPausedGame);
	}
}

void ASTU_PlayerController::OnPausedGame()
{
	if (!GetWorld() || !GetWorld()->GetAuthGameMode()) return;

	GetWorld()->GetAuthGameMode()->SetPause(this);
}

void ASTU_PlayerController::OnMatchStateChanged(ESTU_MatchState State)
{
	if (State == ESTU_MatchState::InProgress)
	{
		bShowMouseCursor = false;
		SetInputMode(FInputModeGameOnly());
	}
	else
	{
		bShowMouseCursor = true;
		SetInputMode(FInputModeUIOnly());
	}
}
