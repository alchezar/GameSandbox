// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ER_GameModeBase.generated.h"

class UER_GameHud;
class AER_FloorTile;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCoinsCountChangedSignature, int32);

UCLASS()
class GAMESANDBOX_API AER_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AER_GameModeBase();
	virtual void Tick(float DeltaTime) override;
	void CreateInitialFloorTiles();
	AER_FloorTile* AddFloorTile(const bool bSpawnObstacles);
	void RestartLevel();
	void AddCoin();
	TArray<FVector> GetLaneMidLocations();

protected:
	virtual void BeginPlay() override;

public:
	FOnCoinsCountChangedSignature OnCoinsCountChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Config")
	TSubclassOf<UUserWidget> GameHudClass;

	UPROPERTY(EditAnywhere, Category = "Kinder | Config")
	TSubclassOf<AER_FloorTile> FloorTileClass;
	UPROPERTY(EditAnywhere, Category = "Kinder | Config")
	int32 InitialFloorTilesNum = 10;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Game")
	float RespawnTime = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Game")
	FName LevelName = "EndlessRunner";

private:
	UPROPERTY()
	UER_GameHud* GameHud;
	FTimerHandle RestartTimer;
	int32 TotalCoins;
	FTransform NextSpawnPointLocation;
	TArray<FVector> LaneMidLocations;
};
