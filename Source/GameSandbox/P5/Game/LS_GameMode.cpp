// Copyright (C) 2023, IKinder

#include "LS_GameMode.h"
#include "P5/Player/LS_PlayerCharacter.h"
#include "P5/Player/LS_PlayerController.h"

ALS_GameMode::ALS_GameMode()
{
	PrimaryActorTick.bCanEverTick = true;

	DefaultPawnClass = ALS_PlayerCharacter::StaticClass();
	PlayerControllerClass = ALS_PlayerController::StaticClass();
}

void ALS_GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void ALS_GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

