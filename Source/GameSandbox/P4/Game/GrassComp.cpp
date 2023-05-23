// Copyright (C) 2023, IKinder

#include "GrassComp.h"

UGrassComp::UGrassComp()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UGrassComp::BeginPlay()
{
	Super::BeginPlay();
	SpawnGrass();
}

void UGrassComp::SpawnGrass()
{
	
	for(int i = 0; i < SpawnCount; ++i)
	{
		FVector Location = FMath::RandPointInBox(SpawningExtends);
		// FRotator Rotation = FRotator(0.f, FMath::RandRange(-180.f, 180.f), 0.f);
		// FVector Scale = FVector(FMath::RandRange(-0.75f, 1.25f));
		
		// AddInstance(FTransform(Rotation, Location, Scale));
		AddInstance(FTransform(Location));
	}
}

