// Copyright (C) 2023, IKinder

#include "Game/P10GameMode.h"

#include "AI/P10TrackerBot.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Game/P10GameState.h"
#include "Player/P10Character.h"
#include "Player/P10PlayerController.h"
#include "Player/P10PlayerState.h"
#include "UI/P10HUD.h"

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

void AP10GameMode::CompleteMission(APawn* InstigatorPawn, const bool bSuccess, const float ShowTime)
{
	SetWaveState(EP10WaveState::GameOver);
	if (!InstigatorPawn) return;

	FString PrintMsg = UEnum::GetValueAsString(InstigatorPawn->GetRemoteRole()).RightChop(5) + ": ";
	PrintMsg += bSuccess ? "MISSION COMPLETE" : "MISSION FAILED";
	const FColor MsgColor = bSuccess ? FColor::Green : FColor::Red;
	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 10.f, MsgColor, PrintMsg);

	if (AP10GameState* CurrentGameState = GetGameState<AP10GameState>())
	{
		CurrentGameState->Multicast_OnMissionComplete(InstigatorPawn, bSuccess, ShowTime);
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
	if (Locations.IsEmpty() || !TrackerBotClass || !AdvancedAIClass) return;

	TSubclassOf<APawn> CurrentEnemyClass = TrackerBotClass;
	if (EnemyClass == EP10EnemyClass::AdvancedAI)
	{
		CurrentEnemyClass = AdvancedAIClass;
	}
	// TODO: This is a bad idea as long as our enemies are not derives from the same class.

	const FTransform SpawnTransform = {FRotator::ZeroRotator, Locations[0]};
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
	if (APawn* TrackerBot = GetWorld()->SpawnActor<APawn>(CurrentEnemyClass, SpawnTransform, Params))
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

void AP10GameMode::RestartAttempt(AP10Character* DeadChar)
{
	/* Find the number of alive characters. */
	int32 AliveCharactersNum = 0;
	DeadPlayerControllers.Empty();
	for (auto It = GetWorld()->GetPlayerControllerIterator(); It; ++It)
	{
		APlayerController* PlayerController = It->Get();
		if (!PlayerController) continue;
		const AP10Character* Char = Cast<AP10Character>(PlayerController->GetPawn());
		
		if (PlayerController->GetPawn() && Char && !Char->GetIsDead())
		{
			++AliveCharactersNum;
			continue;
		}

		/* At this point current iterated controller has no alive pawn, so we can add it to the array. */
		DeadPlayerControllers.Add(PlayerController);
	}
	if (AliveCharactersNum > 0) return;

	/* At this point - all characters are dead, so we can reset values to default. */
	constexpr float RestartDelay = 5.f;
	WaveCount = 0;
	BotsToSpawn = 0;

	/* Kill all bots still alive. */
	if (EnemyClass == EP10EnemyClass::AdvancedAI)
	{
		while (!SpawnedBots.IsEmpty())
		{
			AP10TrackerBot* TrackerBot = Cast<AP10TrackerBot>(SpawnedBots[0]);
			if (!TrackerBot) continue;
			
			TrackerBot->ForceSuicide();
		}
	}

	/* Hide widgets and reset score for all dead characters before showing CompleteMission widget. */
	for (const APlayerController* DeadPlayerController : DeadPlayerControllers)
	{
		AP10HUD* DeadPlayerHUD = Cast<AP10HUD>(DeadPlayerController->GetHUD());
		ensure(DeadPlayerHUD);
		AP10PlayerState* DeadPlayerState = DeadPlayerController->GetPlayerState<AP10PlayerState>();
		ensure(DeadPlayerState);
		
		DeadPlayerHUD->PlayerDied();
		DeadPlayerState->ResetScore();
	}
	CompleteMission(DeadChar, false, RestartDelay);

	/* After widget ShowTime delay - respawn all dead characters. */
	FTimerHandle RestartTimer;
	FTimerDelegate RestartDelegate;
	RestartDelegate.BindUObject(this, &ThisClass::RespawnDeadPlayers);
	GetWorld()->GetTimerManager().SetTimer(RestartTimer, RestartDelegate, RestartDelay, false);
}

void AP10GameMode::RespawnDeadPlayers()
{
	for (APlayerController* DeadPlayerController : DeadPlayerControllers)
	{
		if (!DeadPlayerController) continue;
		AP10HUD* PlayerHUD = Cast<AP10HUD>(DeadPlayerController->GetHUD());
		if (!PlayerHUD) continue;;
		
		RestartPlayer(DeadPlayerController);
		PlayerHUD->PlayerSpawned();
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
	if (!CurrentGameState) return;

	CurrentGameState->SetWaveState(NewState);
}

void AP10GameMode::OnActorKilledHandle(AActor* Victim, AActor* Killer, AController* KillerInstigator)
{
	if (!Victim || !Killer) return;
	const APawn* VictimPawn = Cast<APawn>(Victim);
	if (!VictimPawn || VictimPawn->IsPlayerControlled()) return;	
	const APawn* KillerPawn = Cast<APawn>(Killer);
	if (!KillerPawn) return;
	AP10PlayerState* KillerPlayerState = KillerPawn->GetPlayerState<AP10PlayerState>();
	if (!KillerPlayerState) return;
	
	KillerPlayerState->AddScore(1.f);
	if (GEngine) GEngine->AddOnScreenDebugMessage(2, 6.f, FColor::Yellow, FString::Printf(TEXT("Score: %s"), *FString::SanitizeFloat(KillerPlayerState->GetScore())));
}
