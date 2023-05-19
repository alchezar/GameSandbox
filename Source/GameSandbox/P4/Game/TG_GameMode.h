// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Game/ER_GameModeBase.h"
#include "TG_GameMode.generated.h"

class ATG_TerrainTile;

UCLASS()
class GAMESANDBOX_API ATG_GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATG_GameMode();
	virtual void Tick(float DeltaTime) override;

	void AddInitialTiles();
	void AddNextTile();
	void RemovePreviousTile();
	void UpdateCurrentTileIndex();
	void UpdateActiveTiles(); 

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditAnywhere, Category = "Kinder | Terrain")
	TSubclassOf<ATG_TerrainTile> TerrainTileClass;
	UPROPERTY(EditAnywhere, Category = "Kinder | Terrain")
	FVector NextSpawnLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Kinder | Terrain")
	int32 InitialTilesNum = 1;
	
private:
	UPROPERTY()
	TArray<ATG_TerrainTile*> SpawnedTiles;
	int32 CurrentTile = -1;
};
