// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ARGameModeBase.generated.h"

class UARSaveGame;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

UCLASS()
class GAMESANDBOX_API AARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARGameModeBase();
	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void StartPlay() override;

	virtual void HandleStartingNewPlayer_Implementation(APlayerController* NewPlayer) override;

	void WriteSaveGame();
	void LoadSaveGame();
	
	UFUNCTION()
	void OnPlayerKilled(AActor* Victim, AActor* Killer);

protected:
	void SpawnBot();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);

private:
	UFUNCTION(Exec)
	void KillAll();
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	float SpawnInterval = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	UEnvQuery* SpawnBotQuery;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	TSubclassOf<AActor> MinionClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	UCurveFloat* DifficultyCurve;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	int32 MaxBotNum = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	int32 CreditsPerKill = 5;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Save")
	UARSaveGame* CurrentSaveGame;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Save")
	FString SlotName = "SaveGame01";
	
private:
	FTimerHandle SpawnBotTimer;
};
