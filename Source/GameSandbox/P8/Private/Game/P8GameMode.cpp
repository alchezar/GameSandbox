// Copyright (C) 2023, IKinder

#include "P8/Public/Game/P8GameMode.h"

AP8GameMode::AP8GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP8GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AP8GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

