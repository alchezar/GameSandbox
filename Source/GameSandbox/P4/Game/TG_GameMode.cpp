// Copyright (C) 2023, IKinder

#include "TG_GameMode.h"
#include "TG_TerrainTile.h"
#include "EngineUtils.h"

ATG_GameMode::ATG_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATG_GameMode::BeginPlay()
{
	Super::BeginPlay();
	AddInitialTiles();
}

void ATG_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATG_GameMode::AddInitialTiles()
{
	// Spawn tiles with locked barrier
	for (int i = 0; i < InitialTilesNum; ++i)
	{
		AddNextTile();
	}
	// Unlock first barrier
	for (const auto Tile : TActorRange<ATG_TerrainTile>(GetWorld()))
	{
		if (Tile->GetIsAlwaysUnlocked() && !SpawnedTiles.IsEmpty())
		{
			Tile->SetNextTile(SpawnedTiles[0]);
		}
	}
}

void ATG_GameMode::AddNextTile()
{
	const auto TerrainTile = GetWorld()->SpawnActor<ATG_TerrainTile>(TerrainTileClass, NextSpawnLocation, FRotator::ZeroRotator);
	NextSpawnLocation = TerrainTile->GetAttachPoint();

	// Some sort of linked list of tiles
	if (!SpawnedTiles.IsEmpty())
	{
		if (ATG_TerrainTile* PreviousTile = SpawnedTiles.Last())
		{
			PreviousTile->SetNextTile(TerrainTile);
		}
	}
	SpawnedTiles.Add(TerrainTile);
}

void ATG_GameMode::RemovePreviousTile()
{
	if (CurrentTile <= 1) return;

	SpawnedTiles[CurrentTile - 2]->Destroy();
	SpawnedTiles[CurrentTile - 2] = nullptr;
}

void ATG_GameMode::UpdateCurrentTileIndex()
{
	++CurrentTile;
}

void ATG_GameMode::UpdateActiveTiles()
{
	AddNextTile();
	UpdateCurrentTileIndex();
	RemovePreviousTile();
}
