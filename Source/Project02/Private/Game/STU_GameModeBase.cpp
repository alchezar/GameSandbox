// Copyright (C) 2023, IKinder

#include "Game/STU_GameModeBase.h"

#include "AIController.h"
#include "EngineUtils.h"
#include "Component/STU_HealthComponent.h"
#include "Component/STU_RespawnComponent.h"
#include "Component/STU_WeaponComponent.h"
#include "Player/STU_BaseCharacter.h"
#include "Player/STU_PlayerController.h"
#include "Player/STU_PlayerStart.h"
#include "Player/STU_PlayerState.h"
#include "UI/STU_GameHUD.h"

constexpr static int32 MinRoundTimeForRespawn = 10;

ASTU_GameModeBase::ASTU_GameModeBase()
{
	DefaultPawnClass      = ASTU_BaseCharacter::StaticClass();
	PlayerControllerClass = ASTU_PlayerController::StaticClass();
	HUDClass              = ASTU_GameHUD::StaticClass();
	PlayerStateClass      = ASTU_PlayerState::StaticClass();
}

void ASTU_GameModeBase::StartPlay()
{
	Super::StartPlay();

	SpawnTeams();
	CurrentRound = 1;
	StartRound();
	SetMatchState(ESTU_MatchState::InProgress);
}

void ASTU_GameModeBase::SpawnTeams()
{
	if (!GetWorld()) return;
	int32 TeamID = 1;

	// Player
	for (const auto Player : TActorRange<ASTU_PlayerController>(GetWorld()))
	{
		SetupTeammate(Player, TeamID);
		SetPlayerColor(Player);
	}

	// Bots
	for (int32 i = 0; i < GameData.PlayersNumber - 1; ++i)
	{
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		AAIController* AiController = GetWorld()->SpawnActor<AAIController>(AIControllerClass, SpawnInfo);

		SetupTeammate(AiController, TeamID);
		RestartPlayer(AiController);
		SetPlayerColor(AiController);
	}
}

void ASTU_GameModeBase::SetupTeammate(const AController* Controller, int32& TeamID)
{
	ASTU_PlayerState* PlayerState = Cast<ASTU_PlayerState>(Controller->PlayerState);
	if (!PlayerState) return;

	PlayerState->SetTeamID(TeamID);
	PlayerState->SetTeamColor(GetColorByTeamID(TeamID, GameData.TeamColors, GameData.DefaultTeamColor));
	PlayerState->SetBlasterColor(GetColorByTeamID(TeamID, GameData.BlasterColors, GameData.DefaultBlasterColor));
	PlayerState->SetPlayerName(Controller->IsPlayerController() ? "Player" : "Bot");
	TeamID = TeamID == 1 ? 2 : 1;
}

void ASTU_GameModeBase::StartRound()
{
	RoundCountDown = GameData.RoundTime;
	GetWorldTimerManager().SetTimer(GameRoundHandle, this, &ThisClass::GameTimerUpdate, 1.f, true);
}

void ASTU_GameModeBase::GameTimerUpdate()
{
	if (--RoundCountDown == 0)
	{
		GetWorldTimerManager().ClearTimer(GameRoundHandle);
		if (CurrentRound + 1 <= GameData.RoundsNum)
		{
			++CurrentRound;
			ResetPlayers();
			StartRound();
		}
		else
		{
			GameOver();
		}
	}
}

void ASTU_GameModeBase::ResetPlayers()
{
	if (!GetWorld()) return;

	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		ResetOnePlayer(It->Get());
	}
}

void ASTU_GameModeBase::ResetOnePlayer(AController* Controller)
{
	if (Controller && Controller->GetPawn())
	{
		Controller->GetPawn()->Reset();
	}
	RestartPlayer(Controller);
	SetPlayerColor(Controller);
}

FLinearColor ASTU_GameModeBase::GetColorByTeamID(const int32 TeamID, const TArray<FLinearColor>& Colors, const FLinearColor& Default) const
{
	if (TeamID - 1 < Colors.Num())
	{
		return Colors[TeamID - 1];
	}
	return Default;
}

void ASTU_GameModeBase::SetPlayerColor(const AController* Controller) const
{
	if (!Controller) return;

	ASTU_BaseCharacter* Character = Cast<ASTU_BaseCharacter>(Controller->GetPawn());
	if (!Character) return;

	const ASTU_PlayerState* PlayerState = Cast<ASTU_PlayerState>(Controller->PlayerState);
	if (!PlayerState) return;

	Character->SetPlayerColor(PlayerState->GetTeamColor());
}

void ASTU_GameModeBase::StopRoundWhenAllTeamDead()
{
	int32 LastTeamID = 0;
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const AController* Controller = It->Get();
		if (!Controller) return;

		const ASTU_PlayerState* PlayerState = Cast<ASTU_PlayerState>(Controller->PlayerState);
		if (!PlayerState) return;

		const auto Character = Cast<ASTU_BaseCharacter>(Controller->GetPawn());
		if (!Character || Character->FindComponentByClass<USTU_HealthComponent>()->IsDead()) continue;

		int32 TeamID = PlayerState->GetTeamID();
		LastTeamID   = LastTeamID == 0 ? TeamID : LastTeamID;
		if (LastTeamID != TeamID) return;
	}
	RoundCountDown = RoundCountDown > 5 ? 5 : RoundCountDown;
}

void ASTU_GameModeBase::Killed(const AController* KillerController, const AController* VictimController)
{
	ASTU_PlayerState* KillerPlayerState = KillerController ? Cast<ASTU_PlayerState>(KillerController->PlayerState) : nullptr;
	ASTU_PlayerState* VictimPlayerState = VictimController ? Cast<ASTU_PlayerState>(VictimController->PlayerState) : nullptr;

	const bool bFriendlyFire = KillerPlayerState && VictimPlayerState && KillerPlayerState->GetTeamID() == VictimPlayerState->GetTeamID();

	if (KillerController && !bFriendlyFire)
	{
		KillerPlayerState->AddKill();
	}
	if (VictimController && !bFriendlyFire)
	{
		VictimPlayerState->AddDeath();
	}
	StartRespawn(VictimController);
}

void ASTU_GameModeBase::LogPlayerInfo()
{
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const AController* Controller = It->Get();
		if (!Controller) continue;
		ASTU_PlayerState* PlayerState = Cast<ASTU_PlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;;

		if (PlayerState->GetTeamID() == 1)
		{
			PlayerState->LogInfo();
		}
	}
	for (auto It = GetWorld()->GetControllerIterator(); It; ++It)
	{
		const AController* Controller = It->Get();
		if (!Controller) continue;;
		ASTU_PlayerState* PlayerState = Cast<ASTU_PlayerState>(Controller->PlayerState);
		if (!PlayerState) continue;;

		if (PlayerState->GetTeamID() == 2)
		{
			PlayerState->LogInfo();
		}
	}
}

FGameData ASTU_GameModeBase::GetGameData() const
{
	return GameData;
}

int32 ASTU_GameModeBase::GetCurrentRound() const
{
	return CurrentRound;
}

int32 ASTU_GameModeBase::GetRoundCountDown() const
{
	return RoundCountDown;
}

void ASTU_GameModeBase::StartRespawn(const AController* RespawnController) const
{
	const bool bRespawnAvailable = RoundCountDown > MinRoundTimeForRespawn + GameData.RespawnTime;
	if (!bRespawnAvailable) return;

	if (!RespawnController) return;

	USTU_RespawnComponent* RespawnComponent = RespawnController->FindComponentByClass<USTU_RespawnComponent>();
	if (!RespawnComponent) return;

	RespawnComponent->Respawn(GameData.RespawnTime);
}

void ASTU_GameModeBase::RespawnRequest(AController* RespawnController)
{
	ResetOnePlayer(RespawnController);
}

void ASTU_GameModeBase::GameOver()
{
	// LogPlayerInfo();

	for (APawn* Pawn : TActorRange<APawn>(GetWorld()))
	{
		if (!Pawn) continue;

		Pawn->TurnOff();
		Pawn->DisableInput(nullptr);

		if (const ASTU_BaseCharacter* Character = Cast<ASTU_BaseCharacter>(Pawn))
		{
			if (USTU_WeaponComponent* WeaponComponent = Character->FindComponentByClass<USTU_WeaponComponent>())
			{
				WeaponComponent->StopFire();
			}
		}
	}
	SetMatchState(ESTU_MatchState::GameOver);
}

UClass* ASTU_GameModeBase::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	if (InController && InController->IsA<AAIController>())
	{
		return AIPawnClass;
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}

AActor* ASTU_GameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<ASTU_PlayerStart*> StartActors;
	for (TActorIterator<ASTU_PlayerStart> It(GetWorld(), ASTU_PlayerStart::StaticClass()); It; ++It)
	{
		ASTU_PlayerStart* Actor = *It;
		StartActors.Add(Actor);
	}

	for (const auto StartActor : StartActors)
	{	
		if (const ASTU_PlayerState* PlayerState = Cast<ASTU_PlayerState>(Player->PlayerState))
		{
			const int32 TeamID = PlayerState->GetTeamID(); // debug
			if (StartActor->TeamID == TeamID)
			{
				return StartActor;
			}
		}
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}

void ASTU_GameModeBase::SetMatchState(const ESTU_MatchState NewMatchState)
{
	if (MatchState == NewMatchState) return;

	MatchState = NewMatchState;
	OnMatchStateChange.Broadcast(NewMatchState);
}

bool ASTU_GameModeBase::SetPause(APlayerController* PC, FCanUnpause CanUnpauseDelegate)
{
	SetMatchState(ESTU_MatchState::Pause);

	const bool bSuperPause = Super::SetPause(PC, CanUnpauseDelegate);
	if (bSuperPause)
	{
		StopAllFire();
		SetMatchState(ESTU_MatchState::Pause);
	}
	return bSuperPause;
}

bool ASTU_GameModeBase::ClearPause()
{
	const bool bPauseCleared = Super::ClearPause();
	if (bPauseCleared)
	{
		SetMatchState(ESTU_MatchState::InProgress);
	}
	return bPauseCleared;
}

void ASTU_GameModeBase::StopAllFire()
{
	for(const auto Pawn : TActorRange<APawn>(GetWorld()))
	{
		USTU_WeaponComponent* WeaponComponent = Pawn->FindComponentByClass<USTU_WeaponComponent>();
		if (!WeaponComponent) continue;

		WeaponComponent->StopFire();
		WeaponComponent->ToggleAim(false);
	}
}