// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ER_GameModeBase.generated.h"

class AER_FloorTile;

UCLASS()
class GAMESANDBOX_API AER_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	AER_GameModeBase();
	virtual void Tick(float DeltaTime) override;

	void CreateInitialFloorTiles();
	AER_FloorTile* AddFloorTile();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere, Category = "Kinder | Config")
	TSubclassOf<AER_FloorTile> FloorTileClass;
	UPROPERTY(EditAnywhere, Category = "Kinder | Config")
	int32 InitialFloorTilesNum = 10;
	
	FTransform NextSpawnPointLocation;
	TArray<float> LaneSwitchValues; 
};
