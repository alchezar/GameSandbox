// Copyright (C) 2023. IKinder

#include "GameSandbox/P1/Character/BatteryCollectorGameMode.h"
#include "EngineUtils.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameSandbox/P1/Character/PickupTrooper.h"
#include "GameSandbox/P1/Spawners/BatterySpawnerBase.h"
#include "GameSandbox/P1/Widget/PlayerHUD.h"
#include "Kismet/GameplayStatics.h"

ABatteryCollectorGameMode::ABatteryCollectorGameMode()
	: Super()
{
	DefaultPawnClass = BatteryCollector;
}

void ABatteryCollectorGameMode::BeginPlay()
{
	Super::BeginPlay();

	FindAllSpawners();

	FTimerHandle PowerDecayTimer;
	GetWorld()->GetTimerManager().SetTimer(PowerDecayTimer, this, &ABatteryCollectorGameMode::StartPowerLevelDecay, DelayTime, true);

	PlayerCharacter = Cast<APickupTrooper>(UGameplayStatics::GetPlayerPawn(this, 0));
	if (PlayerCharacter)
	{
		PowerToWin = PlayerCharacter->GetBasePowerLevel() * PowerToWinMultiplier;
	}
	if (MainHUDClass)
	{
		ActiveWidget = CreateWidget<UPlayerHUD>(GetWorld(), MainHUDClass);
		ActiveWidget->AddToViewport();
		SetGameState(EPickupGameState::Playing);
	}
}

void ABatteryCollectorGameMode::FindAllSpawners()
{
	for (TActorIterator<ABatterySpawnerBase> It(GetWorld()); It; ++It)
	{
		ActiveSpawners.Add(*It);
	}
}

void ABatteryCollectorGameMode::StartPowerLevelDecay()
{
	if (!PlayerCharacter) return;
	if (PlayerCharacter->GetCurrentPowerLevel() > PowerToWin)
	{
		SetGameState(EPickupGameState::Win);
	}
	else if (PlayerCharacter->GetCurrentPowerLevel() > 0)
	{
		PlayerCharacter->UpdateCurrentPowerLevel(-DecayAmount);
		check(GetPowerToWin());
		ActiveWidget->UpdatePowerBar(PlayerCharacter->GetCurrentPowerLevel() / GetPowerToWin());
	}
	else
	{
		SetGameState(EPickupGameState::Lost);
	}
}

float ABatteryCollectorGameMode::GetPowerToWin() const
{
	return PowerToWin;
}

EPickupGameState ABatteryCollectorGameMode::GetGameState() const
{
	return GameState;
}

void ABatteryCollectorGameMode::SetGameState(const EPickupGameState NewGameState)
{
	GameState = NewGameState;
	ActiveWidget->UpdateTextBlock(NewGameState);
	HandleNewState(NewGameState);
}

void ABatteryCollectorGameMode::HandleNewState(const EPickupGameState NewState)
{
	switch (NewState)
	{
	case EPickupGameState::Win: //fallthrough
	case EPickupGameState::Lost:
		for (ABatterySpawnerBase* Spawner : ActiveSpawners)
		{
			Spawner->SetSpawnerActive(false);
		}
		if (APlayerController* Controller = UGameplayStatics::GetPlayerController(this, 0))
		{
			Controller->SetCinematicMode(true, true, false);

			FTimerHandle RestartTimer;
			GetWorld()->GetTimerManager().SetTimer(OUT RestartTimer, Controller, &APlayerController::RestartLevel, GameRestartDelay);
		}
		PlayerCharacter->GetMovementComponent()->MovementState.bCanJump = false;

		break;
	case EPickupGameState::None:    //fallthrough
	case EPickupGameState::Playing: //fallthrough
	default:
		for (ABatterySpawnerBase* Spawner : ActiveSpawners)
		{
			Spawner->SetSpawnerActive(true);
		}
		break;;
	}
}
