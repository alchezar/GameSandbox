// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryInstanceBlueprintWrapper.h"
#include "GameFramework/GameModeBase.h"
#include "P10GameMode.generated.h"

enum class EP10WaveState : uint8;
class AP10Character;
class AP10TrackerBot;
class UEnvQuery;

DECLARE_MULTICAST_DELEGATE_ThreeParams(FP10OnActorKilledSignature, AActor* /*Victim*/, AActor* /*Killer*/, AController* /*Instigator*/)

UENUM()
enum class EP10EnemyClass : uint8
{
	TrackerBot,
	AdvancedAI
};

UCLASS()
class GAMESANDBOX_API AP10GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AP10GameMode();
	virtual void StartPlay() override;
	void CompleteMission(APawn* InstigatorPawn, const bool bSuccess, const float ShowTime = 5.f);
	void UntrackBot(AP10TrackerBot* Bot);
	void RestartAttempt(AP10Character* DeadChar);

protected:
	virtual void BeginPlay() override;

private:
	void SpawnBot();
	UFUNCTION()
	void OnQueryCompletedHandle(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

	void StartWave();
	void SpawnBotHandle();
	void EndWave();
	void WaitNextWave();
	void SetWaveState(const EP10WaveState NewState) const;
	void OnActorKilledHandle(AActor* Victim, AActor* Killer, AController* KillerInstigator);
	void RespawnDeadPlayers();

public:
	FP10OnActorKilledSignature OnActorKilled;
	
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | AI")
	UEnvQuery* SpawnBotQuery = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | AI")
	TSubclassOf<AP10TrackerBot> TrackerBotClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | AI")
	TSubclassOf<AP10Character> AdvancedAIClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | AI")
	EP10EnemyClass EnemyClass = EP10EnemyClass::TrackerBot;
	
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Wave")
	int32 BotsPerWave = 2;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Wave")
	float TimeBetweenWaves = 5.f;

private:
	FTimerHandle SpawnTimer;
	int32 WaveCount = 0;
	int32 BotsToSpawn = 0;
	UPROPERTY()
	TArray<APawn*> SpawnedBots;
	UPROPERTY()
	TArray<APlayerController*> DeadPlayerControllers;
};
