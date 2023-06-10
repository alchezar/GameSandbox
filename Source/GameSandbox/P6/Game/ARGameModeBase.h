// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "GameFramework/GameModeBase.h"
#include "ARGameModeBase.generated.h"

class UEnvQuery;
class UEnvQueryInstanceBlueprintWrapper;
class UCurveFloat;

UCLASS()
class GAMESANDBOX_API AARGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AARGameModeBase();
	virtual void StartPlay() override;

protected:
	void SpawnBot();

	UFUNCTION()
	void OnQueryCompleted(UEnvQueryInstanceBlueprintWrapper* QueryInstance, EEnvQueryStatus::Type QueryStatus);
	
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	float SpawnInterval = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	UEnvQuery* SpawnBotQuery;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	TSubclassOf<AActor> MinionClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | AI")
	UCurveFloat* DifficultyCurve;
	
private:
	FTimerHandle SpawnBotTimer;
};
