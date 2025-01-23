// Copyright (C) 2023, IKinder

#include "Game/P8GameModeKart.h"

#include "Pawn/P8Kart.h"

AP8GameModeKart::AP8GameModeKart()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = AP8Kart::StaticClass();
}

void AP8GameModeKart::BeginPlay()
{
	Super::BeginPlay();
}

void AP8GameModeKart::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

