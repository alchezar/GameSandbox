// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "P13/Public/Library/P13Types.h"
#include "P13GameMode.generated.h"

class AP13GameState;
class AP13EnemySpawnPoint;

UCLASS()
class GAMESANDBOX_API AP13GameMode : public AGameMode
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	AP13GameMode();

protected:
	virtual void BeginPlay() override;
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	/* ------------------------------- This -------------------------------- */
public:
	void RespawnPlayer(AController* NewPlayer);
	void RespawnEnemies();

protected:
	void OnNewPhaseStartedHandle(EP13LevelPhase NewPhase);

private:
	void CacheGameState();
	FString GetCurrentPhaseString();
	
	void FindAllEnemySpawners();
	void StartSpawningEnemies();
	void SpawnEnemies();
	bool CheckCanSpawnEnemy();

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	float EnemiesSpawnDelay = 2.f;

private:
	TArray<AP13EnemySpawnPoint*> SpawnPoints;
	int32 SpawnedEnemiesNum = 0;
	FTimerHandle SpawnTimer;
	TSoftObjectPtr<AP13GameState> CachedGameState;
};
