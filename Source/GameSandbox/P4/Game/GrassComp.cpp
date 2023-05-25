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
		AddInstance(FTransform(Location));
	}
}

