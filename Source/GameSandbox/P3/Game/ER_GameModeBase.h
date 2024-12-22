// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ER_GameModeBase.generated.h"

class UER_MainMenu;
class UER_GameOver;
class AER_CoinItem;
class AER_Obstacle;
class AER_Character;
class UER_GameHud;
class AER_FloorTile;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnCoinsCountChangedSignature, int32);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnLivesCountChangedSignature, int32);

USTRUCT(BlueprintType)
struct FObstacle
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	TSubclassOf<AER_Obstacle> Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	float Probability = 0.5f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	bool bLarge = false;
};

USTRUCT(BlueprintType)
struct FCollect
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	TSubclassOf<AER_CoinItem> Class;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Spawn")
	float Probability = 0.5f;
};

USTRUCT(BlueprintType)
struct FLevelNames
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Game = "EndlessRunner";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName Start = "EndlessRunner_Start";
};

UCLASS()
class GAMESANDBOX_API AER_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AER_GameModeBase();
	virtual void Tick(float DeltaTime) override;

	void            CreateInitialFloorTiles();
	AER_FloorTile*  AddFloorTile(const bool bSpawnObstacles);
	void            RemoveFloorTile(AER_FloorTile* Tile);
	void            StartFromBegin(AER_Character* DiedCharacter);
	
	TArray<FVector> GetLaneMidLocations();
	FName           GetGameLevelName() const;
	FName           GetMainLevelName() const;

	void AddCoin();
	void DecreaseLives();
	void IncreaseLives();

	TArray<FObstacle> GetObstacles() const;
	FCollect GetCoin() const;

protected:
	virtual void BeginPlay() override;

public:
	FOnCoinsCountChangedSignature OnCoinsCountChanged;
	FOnLivesCountChangedSignature OnLivesCountChanged;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Widget")
	TSubclassOf<UUserWidget> GameHudClass;
	UPROPERTY(EditDefaultsOnly, Category = "Kinder | Widget")
	TSubclassOf<UUserWidget> GameOverClass;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Game")
	int32 MaxLives = 3;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Game")
	int32 LifePrice = 100;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Game")
	float RespawnTime = 2.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Kinder | Game")
	FLevelNames LevelNames;
	
	UPROPERTY(EditAnywhere, Category = "Kinder | Spawn")
	TSubclassOf<AER_FloorTile> FloorTileClass;
	UPROPERTY(EditAnywhere, Category = "Kinder | Spawn")
	int32 InitialFloorTilesNum = 10;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Spawn")
	TArray<FObstacle> Obstacles;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Kinder | Spawn")
	FCollect Coin;

private:
	UPROPERTY()
	UER_GameHud* GameHud;
	
	FTimerHandle RestartTimer;
	int32 TotalCoins = 0;
	int32 LivesCount = 3;
	int32 Score = 0;
	FTransform NextSpawnPointLocation;
	TArray<FVector> LaneMidLocations;
	TArray<AER_FloorTile*> FloorTiles;
};
