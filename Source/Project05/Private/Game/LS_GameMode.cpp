// Copyright (C) 2023, IKinder

#include "Game/LS_GameMode.h"

#include "Player/LS_PlayerCharacter.h"
#include "Player/LS_PlayerController.h"

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

