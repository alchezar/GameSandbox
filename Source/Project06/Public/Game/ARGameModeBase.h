// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ARGameModeBase.generated.h"

class UDataTable;
class UARSaveGame;
class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;
class UARMonsterData;

USTRUCT(BlueprintType)
struct FMonsterInfoRow : public FTableRowBase
{
	GENERATED_BODY()

	FMonsterInfoRow()
	{
		// MonsterData = nullptr;
		Weight = 1.f;
		SpawnCost = 5.f;
		KillReward = 20.f;
		TeamColor = FLinearColor::White;
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite) // Don`t use Category property, because we won`t be able to edit DataTable in editor 
	FPrimaryAssetId MonsterId;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Weight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SpawnCost;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KillReward;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FLinearColor TeamColor;
};

UCLASS()
class PROJECT06_API AARGameModeBase : public AGameModeBase
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
	void OnMonsterLoaded(const FPrimaryAssetId LoadedId, const FVector SpawnLocation, const FLinearColor Color);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	float SpawnInterval = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	UEnvQuery* SpawnBotQuery = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	UCurveFloat* DifficultyCurve = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	int32 MaxBotNum = 10;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	int32 CreditsPerKill = 5;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | AI")
	bool bUseTable = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | AI", meta = (EditCondition = "!bUseTable"))
	TSubclassOf<AActor> MinionClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | AI", meta = (EditCondition = "bUseTable"))
	UDataTable* MonsterTable = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Save")
	UARSaveGame* CurrentSaveGame = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Save")
	FString SlotName = "SaveGame01";

private:
	FTimerHandle SpawnBotTimer;
};
