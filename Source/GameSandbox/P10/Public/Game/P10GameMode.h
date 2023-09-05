// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "P10GameMode.generated.h"

class AP10TrackerBot;
class UEnvQuery;

UCLASS()
class GAMESANDBOX_API AP10GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP10GameMode();
	virtual void StartPlay() override;
	void CompleteMission(APawn* InstigatorPawn, bool bSuccess);
	void UntrackBot(AP10TrackerBot* Bot);

protected:
	virtual void BeginPlay() override;
	void SpawnBot();
	UFUNCTION()
	void OnQueryCompletedHandle(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void StartWave();
	void SpawnBotHandle();
	void EndWave();
	void WaitNextWave();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | AI")
	UEnvQuery* SpawnBotQuery = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | AI")
	TSubclassOf<AP10TrackerBot> TrackerBotClass;

	UPROPERTY(EditDefaultsOnly, Category = "C++ | Wave")
	int32 BotsPerWave = 2;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Wave")
	float TimeBetweenWaves = 5.f;

private:
	FTimerHandle SpawnTimer;
	int32 WaveCount = 0;
	int32 BotsToSpawn = 0;
	TArray<AP10TrackerBot*> SpawnedBots;
};
