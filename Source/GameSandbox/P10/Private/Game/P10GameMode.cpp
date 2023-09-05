// Copyright (C) 2023, IKinder

#include "P10/Public/Game/P10GameMode.h"

#include "EnvironmentQuery/EnvQueryManager.h"
#include "P10/Public/AI/P10TrackerBot.h"
#include "P10/Public/Game/P10GameState.h"
#include "P10/Public/Player/P10Character.h"
#include "P10/Public/Player/P10PlayerController.h"
#include "P10/Public/UI/P10HUD.h"

AP10GameMode::AP10GameMode()
{
	DefaultPawnClass = AP10Character::StaticClass();
	HUDClass = AP10HUD::StaticClass();
	GameStateClass = AP10GameState::StaticClass();
	PlayerControllerClass = AP10PlayerController::StaticClass();
}

void AP10GameMode::BeginPlay()
{
	Super::BeginPlay();

	StartWave();
}

void AP10GameMode::StartPlay()
{
	Super::StartPlay();
}

void AP10GameMode::CompleteMission(APawn* InstigatorPawn, bool bSuccess)
{
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

void AP10GameMode::StartWave()
{
	BotsToSpawn = ++WaveCount * BotsPerWave;
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ThisClass::SpawnBotHandle, 1.f, true);
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("Start Wawe %d"), WaveCount));
}

void AP10GameMode::SpawnBotHandle()
{
	SpawnBot();

	if (--BotsToSpawn <= 0)
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}
}

void AP10GameMode::EndWave()
{
	if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Purple, FString::Printf(TEXT("End Wawe")));
	WaitNextWave();
}

void AP10GameMode::WaitNextWave()
{
	FTimerHandle NextWaveTimer;
	GetWorld()->GetTimerManager().SetTimer(NextWaveTimer, this, &ThisClass::StartWave, TimeBetweenWaves, false);
}
