// Copyright (C) 2023, IKinder

#include "Game/ER_GameModeBase.h"

#include "Blueprint/UserWidget.h"
#include "Content/ER_FloorTile.h"
#include "Kismet/GameplayStatics.h"
#include "Player/ER_Character.h"
#include "UI/ER_GameHud.h"
#include "UI/ER_GameOver.h"

AER_GameModeBase::AER_GameModeBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AER_GameModeBase::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->bShowMouseCursor = true;

	GameHud = Cast<UER_GameHud>(CreateWidget(GetWorld(), GameHudClass));
	check(GameHud);
	GameHud->AddToViewport();
	GameHud->InitializeHud(this);
	GameHud->SetLivesCount(LivesCount);

	CreateInitialFloorTiles();
	LivesCount = MaxLives;
}

void AER_GameModeBase::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AER_GameModeBase::CreateInitialFloorTiles()
{
	if (const AER_FloorTile* Tile = AddFloorTile(false))
	{
		LaneMidLocations = Tile->GetLaneMidLocations();
	}

	for (int i = 1; i < InitialFloorTilesNum; ++i)
	{
		// Don`t spawn obstacles on first three tiles
		AddFloorTile(i > 2);
	}
}

AER_FloorTile* AER_GameModeBase::AddFloorTile(const bool bSpawnObstacles)
{
	if (!GetWorld()) return nullptr;

	AER_FloorTile* FloorTile = GetWorld()->SpawnActor<AER_FloorTile>(FloorTileClass, NextSpawnPointLocation);
	if (!FloorTile) return nullptr;

	FloorTiles.Add(FloorTile);
	if (bSpawnObstacles)
	{
		FloorTile->SpawnItems();
	}

	NextSpawnPointLocation = FloorTile->GetAttachPoint();
	return FloorTile;
}

void AER_GameModeBase::RemoveFloorTile(AER_FloorTile* Tile)
{
	FloorTiles.Remove(Tile);
}

TArray<FVector> AER_GameModeBase::GetLaneMidLocations()
{
	return LaneMidLocations;
}

FName AER_GameModeBase::GetGameLevelName() const
{
	return LevelNames.Game;
}

FName AER_GameModeBase::GetMainLevelName() const
{
	return LevelNames.Start;
}

void AER_GameModeBase::StartFromBegin(AER_Character* DiedCharacter)
{
	if (LivesCount <= 0)
	{
		UER_GameOver* GameOver = Cast<UER_GameOver>(CreateWidget(GetWorld(), GameOverClass));
		if (!GameOver) return;

		Score = Score ? Score * 100 + TotalCoins : TotalCoins;
		GameOver->ShowFinalScore(Score);
		GameOver->AddToViewport();
		return;
	}

	/* Avoid using ForEachLoop, half of the array will be ignored:
	 * When item[0] will be deleted - item[1] goes to index [0].
	 * So at the next loop index [1] will be with old item[2]. */
	while (!FloorTiles.IsEmpty())
	{
		FloorTiles.Last()->DestroyFloorTile();
	}

	// Spawn new tiles from start
	NextSpawnPointLocation = FTransform();
	CreateInitialFloorTiles();

	// Respawn player at start position
	DiedCharacter->Resurrect();
}

void AER_GameModeBase::AddCoin()
{
	OnCoinsCountChanged.Broadcast(++TotalCoins);

	if (TotalCoins >= LifePrice)
	{
		++Score;
		TotalCoins -= LifePrice;
		OnCoinsCountChanged.Broadcast(TotalCoins);
		IncreaseLives();
	}
}

void AER_GameModeBase::DecreaseLives()
{
	LivesCount = FMath::Clamp(--LivesCount, 0, MaxLives);
	OnLivesCountChanged.Broadcast(LivesCount);
}

void AER_GameModeBase::IncreaseLives()
{
	++LivesCount;
	OnLivesCountChanged.Broadcast(LivesCount);
}

TArray<FObstacle> AER_GameModeBase::GetObstacles() const
{
	return Obstacles;
}

FCollect AER_GameModeBase::GetCoin() const
{
	return Coin;
}
