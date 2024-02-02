// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameMode.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "GameFramework/PlayerState.h"
#include "P13/Public/Actor/P13EnemySpawn.h"
#include "P13/Public/Character/P13CharacterEnemy.h"
#include "P13/Public/Controller/P13PlayerController.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/Game/P13GameState.h"

AP13GameMode::AP13GameMode() {}

void AP13GameMode::RestartPlayer(AController* NewPlayer)
{
	Super::RestartPlayer(NewPlayer);

	/* Save new controller. */
	LoggedControllers.Add(NewPlayer);

	/* Look for saved pawn color. */
	const UP13GameInstance* GameInstance = GetGameInstance<UP13GameInstance>();
	check(GameInstance)
	const APlayerState* PlayerState = NewPlayer->GetPlayerState<APlayerState>();
	check(PlayerState)

	const TMap<FString, FLinearColor>& PlayersColorsMap = GameInstance->GetPlayersColorsMap();
	const FLinearColor* FoundColor = PlayersColorsMap.Find(PlayerState->GetPlayerName());
	if (!FoundColor)
	{
		return;
	}

	/* Send found color to the controlled pawn. */
	if (AP13PlayerController* PlayerController = Cast<AP13PlayerController>(NewPlayer))
	{
		PlayerController->Multicast_UpdatePlayerColor(*FoundColor);
	}
}

void AP13GameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	LoggedControllers.Remove(Exiting);
}

void AP13GameMode::BeginPlay()
{
	Super::BeginPlay();

	CacheGameState();
	FindAllEnemySpawners();
	StartSpawningEnemies();
}

AActor* AP13GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	TArray<APlayerStart*> PlayerStarts;
	for (TActorIterator<APlayerStart> It(GetWorld(), APlayerStart::StaticClass()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		PlayerStarts.Add(PlayerStart);
	}

	auto PredicateLambda = [&](const APlayerStart* Compared) { return Compared->PlayerStartTag == *GetCurrentPhaseString(); };
	if (APlayerStart** FoundItem = PlayerStarts.FindByPredicate(PredicateLambda))
	{
		return *FoundItem;
	}

	return Super::ChoosePlayerStart_Implementation(Player);
}

void AP13GameMode::RespawnPlayer(AController* NewPlayer)
{
	AActor* BestPlayerStart = FindPlayerStart(NewPlayer, GetCurrentPhaseString());
	if (!BestPlayerStart)
	{
		RestartPlayer(NewPlayer);
		return;
	}
	RestartPlayerAtPlayerStart(NewPlayer, BestPlayerStart);
}

void AP13GameMode::RespawnEnemies()
{
	--SpawnedEnemiesNum;
	StartSpawningEnemies();
}

void AP13GameMode::OnNewPhaseStartedHandle(EP13LevelPhase NewPhase)
{
	for (TActorIterator<AP13CharacterEnemy> It(GetWorld()); It; ++It)
	{
		AP13CharacterEnemy* AliveEnemy = *It;
		if (!AliveEnemy || AliveEnemy->GetIsDead())
		{
			continue;
		}
		AliveEnemy->TakeDamage(AliveEnemy->GetHealthReserve(), FDamageEvent{}, nullptr, nullptr);
	}

	StartSpawningEnemies();
}

void AP13GameMode::CacheGameState()
{
	if (CachedGameState.IsValid())
	{
		return;
	}

	CachedGameState = GetGameState<AP13GameState>();
	check(CachedGameState)
	CachedGameState->OnPhaseChanged.AddUObject(this, &ThisClass::OnNewPhaseStartedHandle);
}

FString AP13GameMode::GetCurrentPhaseString()
{
	CacheGameState();

	const EP13LevelPhase LastLevelPhase = CachedGameState->GetLastLevelPhase();
	return UEnum::GetValueAsString(LastLevelPhase).RightChop(16);
}

void AP13GameMode::FindAllEnemySpawners()
{
	SpawnPoints.Empty();
	for (TActorIterator<AP13EnemySpawnPoint> It(GetWorld(), AP13EnemySpawnPoint::StaticClass()); It; ++It)
	{
		AP13EnemySpawnPoint* SpawnPoint = *It;
		if (!SpawnPoint)
		{
			continue;
		}
		SpawnPoints.Add(SpawnPoint);
	}
}

void AP13GameMode::StartSpawningEnemies()
{
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, [this]() { SpawnEnemies(); }, EnemiesSpawnDelay, true, EnemiesSpawnDelay);
}

void AP13GameMode::SpawnEnemies()
{
	/* Each spawn point will try to spawn enemies if the phases match. */
	for (AP13EnemySpawnPoint* SpawnPoint : SpawnPoints)
	{
		if (!CheckCanSpawnEnemy())
		{
			break;
		}
		if (!SpawnPoint->TrySpawnEnemy())
		{
			continue;
		}
		++SpawnedEnemiesNum;
	}
}

bool AP13GameMode::CheckCanSpawnEnemy()
{
	const bool bResult = (SpawnedEnemiesNum < CachedGameState->GetMaxEnemiesOnPhase());

	if (!bResult && GetWorld()->GetTimerManager().IsTimerActive(SpawnTimer))
	{
		GetWorld()->GetTimerManager().ClearTimer(SpawnTimer);
	}

	return bResult;
}
