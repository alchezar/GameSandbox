// Copyright (C) 2023, IKinder

#include "ER_Obstacle.h"

AER_Obstacle::AER_Obstacle()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AER_Obstacle::BeginPlay()
{
	Super::BeginPlay();
}

void AER_Obstacle::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
