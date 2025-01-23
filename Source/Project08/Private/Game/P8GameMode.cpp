// Copyright (C) 2023, IKinder

#include "Game/P8GameMode.h"

AP8GameMode::AP8GameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	/* Get default pawn class to our Blueprint character */
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Project/P7/Blueprint/Player/BP_P7Player"));
	if (PlayerPawnBPClass.Class)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AP8GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AP8GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}