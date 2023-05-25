// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Game/ER_GameModeBase.h"
#include "TG_GameMode.generated.h"

class ATG_FirstPersonCharacter;
class ATG_BaseCharacter;
class ANavMeshBoundsVolume;
class ATG_TerrainTile;
class UTG_ActorPool;
class UTG_WidgetMain;

DECLARE_MULTICAST_DELEGATE_OneParam(FTGOnScoreChangedSignature, int32);

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
	void PopulateBoundsVolumePool();

	UTG_WidgetMain* GetMainWidget() const;

protected:
	virtual void BeginPlay() override;

private:
	void CreateUI();
	void FindPlayer();

public:
	FTGOnScoreChangedSignature OnScoreChanged;

protected:
	UPROPERTY(EditAnywhere, Category = "Kinder | Terrain")
	TSubclassOf<ATG_TerrainTile> TerrainTileClass;
	UPROPERTY(EditAnywhere, Category = "Kinder | Terrain")
	FVector NextSpawnLocation = FVector::ZeroVector;
	UPROPERTY(EditAnywhere, Category = "Kinder | Terrain")
	int32 InitialTilesNum = 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	TSubclassOf<UUserWidget> MainWidgetClass;

private:
	UPROPERTY()
	TArray<ATG_TerrainTile*> SpawnedTiles;
	int32 CurrentTile = -1;
	UPROPERTY()
	UTG_ActorPool* ActorPool;
	UPROPERTY()
	UTG_WidgetMain* MainWidget;
	UPROPERTY()
	ATG_FirstPersonCharacter* Player;
};
