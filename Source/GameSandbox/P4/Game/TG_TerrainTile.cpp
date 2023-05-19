// Copyright (C) 2023, IKinder

#include "TG_TerrainTile.h"
#include "TG_GameMode.h"
#include "Components/ArrowComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Player/TG_FirstPersonCharacter.h"

ATG_TerrainTile::ATG_TerrainTile()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>("SceneRoot");
	SetRootComponent(SceneRoot);

	BackWall = CreateDefaultSubobject<UStaticMeshComponent>("BackWall");
	BackWall->SetupAttachment(GetRootComponent());

	FrontWall = CreateDefaultSubobject<UStaticMeshComponent>("FrontWall");
	FrontWall->SetupAttachment(GetRootComponent());

	NextTileLocation = CreateDefaultSubobject<UArrowComponent>("NextTileLocation");
	NextTileLocation->SetupAttachment(GetRootComponent());

	LockWall = CreateDefaultSubobject<UStaticMeshComponent>("LockWall");
	LockWall->SetupAttachment(GetRootComponent());
}

void ATG_TerrainTile::BeginPlay()
{
	Super::BeginPlay();
	
	if (!bAlwaysUnlock)
	{
		Lock();
	}
	LockWall->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnLockOverlapped);
	GameMode = Cast<ATG_GameMode>(GetWorld()->GetAuthGameMode());
}

void ATG_TerrainTile::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector ATG_TerrainTile::GetAttachPoint()
{
	return NextTileLocation->GetComponentLocation();
}

bool ATG_TerrainTile::GetIsAlwaysUnlocked() const 
{
	return bAlwaysUnlock;
}

void ATG_TerrainTile::SetNextTile(ATG_TerrainTile* NewNextTile)
{
	NextTile = NewNextTile;
}

void ATG_TerrainTile::OnLockOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(ATG_FirstPersonCharacter::StaticClass()))
	{
		Lock();
		if (GameMode)
		{
			GameMode->UpdateActiveTiles();		
		}
	}
	
}

void ATG_TerrainTile::Lock()
{
	LockWall->SetCollisionProfileName("BlockAll");
	LockWall->SetMaterial(0, LockedAppearance);

	if (NextTile)
	{
		NextTile->Unlock();
	}
}

void ATG_TerrainTile::Unlock()
{
	LockWall->SetCollisionProfileName("OverlapOnlyPawn");
	LockWall->SetMaterial(0, UnlockedAppearance);
}
