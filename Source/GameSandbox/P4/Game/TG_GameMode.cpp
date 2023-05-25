// Copyright (C) 2023, IKinder

#include "TG_GameMode.h"
#include "EngineUtils.h"
#include "TG_ActorPool.h"
#include "TG_TerrainTile.h"
#include "Blueprint/UserWidget.h"
#include "NavMesh/NavMeshBoundsVolume.h"
#include "P4/Widget/TG_WidgetMain.h"
#include "Player/TG_FirstPersonCharacter.h"

ATG_GameMode::ATG_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorPool = CreateDefaultSubobject<UTG_ActorPool>("ActorPool");
}

void ATG_GameMode::BeginPlay()
{
	Super::BeginPlay();
	PopulateBoundsVolumePool();
	AddInitialTiles();
	FindPlayer();
	CreateUI();
}

void ATG_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATG_GameMode::AddInitialTiles()
{
	/* Spawn tiles with locked barrier */
	for (int i = 0; i < InitialTilesNum; ++i)
	{
		AddNextTile();
	}
	/* Unlock first barrier */
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
	NextSpawnLocation      = TerrainTile->GetAttachPoint();
	if (!TerrainTile->GetIsAlwaysUnlocked())
	{
		TerrainTile->SetActorPool(ActorPool);
	}

	/* Some sort of linked list of tiles */
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

	SpawnedTiles[CurrentTile - 2]->DestroyTile();
	SpawnedTiles[CurrentTile - 2] = nullptr;
}

void ATG_GameMode::UpdateCurrentTileIndex()
{
	++CurrentTile;
	OnScoreChanged.Broadcast(CurrentTile);
}

void ATG_GameMode::UpdateActiveTiles()
{
	AddNextTile();
	UpdateCurrentTileIndex();
	RemovePreviousTile();
}

void ATG_GameMode::PopulateBoundsVolumePool()
{
	auto VolumeIterator = TActorIterator<ANavMeshBoundsVolume>(GetWorld());
	while (VolumeIterator)
	{
		ActorPool->Add(*VolumeIterator);
		++VolumeIterator;
	}
}

UTG_WidgetMain* ATG_GameMode::GetMainWidget() const
{
	return MainWidget;
}

void ATG_GameMode::FindPlayer()
{
	Player = *TActorIterator<ATG_FirstPersonCharacter>(GetWorld(), ATG_FirstPersonCharacter::StaticClass());
}

void ATG_GameMode::CreateUI()
{
	MainWidget = CreateWidget<UTG_WidgetMain>(GetWorld(), MainWidgetClass);
	if (!MainWidget) return;

	MainWidget->AddToViewport();
	MainWidget->BindDelegates(this, Player);
	MainWidget->SetScore(CurrentTile);
	MainWidget->SetHealth(Player ? Player->GetMaxHealth() : 0.f);
}
