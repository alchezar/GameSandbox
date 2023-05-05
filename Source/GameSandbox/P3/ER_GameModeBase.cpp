// Copyright (C) 2023, IKinder

#include "ER_GameModeBase.h"
#include "ER_FloorTile.h"

AER_GameModeBase::AER_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AER_GameModeBase::BeginPlay()
{
	Super::BeginPlay();
	
	CreateInitialFloorTiles();
}

void AER_GameModeBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AER_GameModeBase::CreateInitialFloorTiles()
{
	
	if (const AER_FloorTile* Tile = AddFloorTile())
	{
		LaneSwitchValues = Tile->GetLaneShiftValues();
	}	
	
	for (int i = 1; i < InitialFloorTilesNum; ++i)
	{
		AddFloorTile();
	}
}

AER_FloorTile* AER_GameModeBase::AddFloorTile()
{
	if (!GetWorld()) return nullptr;

	AER_FloorTile* FloorTile = GetWorld()->SpawnActor<AER_FloorTile>(FloorTileClass, NextSpawnPointLocation);
	if (!FloorTile) return nullptr;

	NextSpawnPointLocation = FloorTile->GetAttachPoint();
	return FloorTile;
}
