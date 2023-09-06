// Copyright (C) 2023, IKinder

#include "P10/Public/Game/P10GameMode.h"

#include "EngineUtils.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "P10/Public/AI/P10TrackerBot.h"
#include "P10/Public/Game/P10GameState.h"
#include "P10/Public/Player/P10Character.h"
#include "P10/Public/Player/P10PlayerController.h"
#include "P10/Public/Player/P10PlayerState.h"
#include "P10/Public/UI/P10HUD.h"

AP10GameMode::AP10GameMode()
{
	DefaultPawnClass = AP10Character::StaticClass();
	HUDClass = AP10HUD::StaticClass();
	GameStateClass = AP10GameState::StaticClass();
	PlayerControllerClass = AP10PlayerController::StaticClass();
	PlayerStateClass = AP10PlayerState::StaticClass();
}

void AP10GameMode::BeginPlay()
{
	Super::BeginPlay();

	OnActorKilled.AddUObject(this, &ThisClass::OnActorKilledHandle);
	StartWave();
}

void AP10GameMode::StartPlay()
{
	Super::StartPlay();
}

void AP10GameMode::CompleteMission(APawn* InstigatorPawn, bool bSuccess)
{
	SetWaveState(EP10WaveState::GameOver);
	if (!InstigatorPawn) return;

	FString PrintMsg = UEnum::GetValueAsString(InstigatorPawn->GetRemoteRole()).RightChop(5) + ": ";
	PrintMsg += bSuccess ? "MISSION COMPLETE" : "MISSION FAILED";
	const FColor MsgColor = bSuccess ? FColor::Green : FColor::Red;
	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 10.f, MsgColor, PrintMsg);

	if (AP10GameState* CurrentGameState = GetGameState<AP10GameState>())
	{
		CurrentGameState->Multicast_OnMissionComplete(InstigatorPawn, bSuccess);
	}
}

void AP10GameMode::SpawnBot()
{
	UEnvQueryInstanceBlueprintWrapper* QueryInstance = UEnvQueryManager::RunEQSQuery(this, SpawnBotQuery, this, EEnvQueryRunMode::RandomBest25Pct, nullptr);
	if (!QueryInstance) return;

	QueryInstance->GetOnQueryFinishedEvent().AddDynamic(this, &ThisClass::OnQueryCompletedHandle);
}

void AP10GameMode::OnQueryCompletedHandle(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus)
{
	if (QueryStatus != EEnvQueryStatus::Success) return;

	TArray<FVector> Locations = QueryInstance->GetResultsAsLocations();
	if (Locations.IsEmpty() || !TrackerBotClass) return;

	const FTransform SpawnTransform = {FRotator::ZeroRotator, Locations[0]};
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	if (AP10TrackerBot* TrackerBot = GetWorld()->SpawnActor<AP10TrackerBot>(TrackerBotClass, SpawnTransform, Params))
	{
		SpawnedBots.Add(TrackerBot);
	}
}

void AP10GameMode::UntrackBot(AP10TrackerBot* Bot)
{
	SpawnedBots.Remove(Bot);

	if (SpawnedBots.IsEmpty())
	{
		EndWave();
	}
}

void AP10GameMode::CheckAnyPlayerStillAlive(AP10Character* DeadChar)
{
	TArray<AP10Character*> AliveCharacters;
	for(auto AliveCharacter : TActorRange<AP10Character>(GetWorld()))
	{
		if (!AliveCharacter || AliveCharacter->GetIsDead() || AliveCharacter == DeadChar) continue;

		AliveCharacters.Add(AliveCharacter);
	}
	if (AliveCharacters.IsEmpty())
	{
		CompleteMission(DeadChar, false);
	}
}

void AP10GameMode::StartWave()
{
	SetWaveState(EP10WaveState::InProgress);
	BotsToSpawn = ++WaveCount * BotsPerWave;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ThisClass::SpawnBotHandle, 1.f, true);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Start Wawe %d"), WaveCount));
}

void AP10GameMode::SpawnBotHandle()
{
	SpawnBot();

	if (--BotsToSpawn <= 0)
	{
		SetWaveState(EP10WaveState::WaitingToComplete);
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}

void AP10GameMode::EndWave()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("End Wawe")));
	SetWaveState(EP10WaveState::Complete);
	WaitNextWave();
}

void AP10GameMode::WaitNextWave()
{
	SetWaveState(EP10WaveState::WaitingToStart);
	FTimerHandle NextWaveTimer;
	GetWorld()->GetTimerManager().SetTimer(NextWaveTimer, this, &ThisClass::StartWave, TimeBetweenWaves, false);
}

void AP10GameMode::SetWaveState(const EP10WaveState NewState) const
{
	AP10GameState* CurrentGameState = GetWorld()->GetGameState<AP10GameState>();
	if (!ensure(CurrentGameState)) return;

	CurrentGameState->SetWaveState(NewState);
}

void AP10GameMode::OnActorKilledHandle(AActor* Victim, AActor* Killer, AController* KillerInstigator)
{
	if (!Victim || !Killer) return;
	const APawn* VictimPawn = Cast<APawn>(Victim);
	if (!VictimPawn || VictimPawn->IsPlayerControlled()) return;
	// const APawn* KillerPawn = KillerInstigator->GetPawn();
	const APawn* KillerPawn = Cast<APawn>(Killer);
	if (!KillerPawn) return;
	AP10PlayerState* KillerPlayerState = KillerPawn->GetPlayerState<AP10PlayerState>();
	if (!KillerPlayerState) return;
	
	KillerPlayerState->AddScore(1.f);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 6.f, FColor::Yellow, FString::Printf(TEXT("Score: %f"), KillerPlayerState->GetScore()));
}
