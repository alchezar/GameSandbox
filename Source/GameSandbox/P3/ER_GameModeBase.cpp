// Copyright (C) 2023, IKinder

#include "ER_GameModeBase.h"
#include "ER_FloorTile.h"
#include "ER_GameHud.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

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
	
	CreateInitialFloorTiles();
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

	if (bSpawnObstacles)
	{
		FloorTile->SpawnItems();
	}

	NextSpawnPointLocation = FloorTile->GetAttachPoint();
	return FloorTile;
}

void AER_GameModeBase::RestartLevel()
{
	FTimerDelegate RestartDelegate;
	RestartDelegate.BindLambda([&]()
	{
		GetWorldTimerManager().ClearTimer(RestartTimer);
		UGameplayStatics::OpenLevel(GetWorld(), LevelName);
	});
	GetWorldTimerManager().SetTimer(RestartTimer, RestartDelegate, RespawnTime, false);
}

void AER_GameModeBase::AddCoin()
{	
	OnCoinsCountChanged.Broadcast(++TotalCoins);
}

TArray<FVector> AER_GameModeBase::GetLaneMidLocations()
{
	return LaneMidLocations; 
}
