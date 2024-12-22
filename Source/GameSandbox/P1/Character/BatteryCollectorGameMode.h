// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "BatteryCollectorGameMode.generated.h"

class ABatterySpawnerBase;
class APickupTrooper;
class UUserWidget;
class UPlayerHUD;

UENUM(BlueprintType)
enum class EPickupGameState : uint8
{
	Playing,
	Win,
	Lost,
	None
};

UCLASS()
class GAMESANDBOX_API ABatteryCollectorGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	ABatteryCollectorGameMode();

	void SetGameState(EPickupGameState NewGameState);

	UFUNCTION(BlueprintPure, Category = "Kinder")
	float GetPowerToWin() const;
	UFUNCTION(BlueprintPure, Category = "Kinder")
	EPickupGameState GetGameState() const;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<APickupTrooper> BatteryCollector;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder")
	float GameRestartDelay = 10.f;	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder")
	float DelayTime = 0.2f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder")
	float DecayAmount = 7.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder")
	float PowerToWin = 0.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder")
	float PowerToWinMultiplier = 1.5f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Kinder")
	TSubclassOf<UPlayerHUD> MainHUDClass;
	UPROPERTY()
	UPlayerHUD* ActiveWidget;

private:
	void FindAllSpawners();
	void StartPowerLevelDecay();
	void HandleNewState(EPickupGameState NewState);

	EPickupGameState GameState;

	UPROPERTY()
	APickupTrooper* PlayerCharacter;
	UPROPERTY()
	TArray<ABatterySpawnerBase*> ActiveSpawners;
};
