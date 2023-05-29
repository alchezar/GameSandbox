// Copyright (C) 2023, IKinder

#include "LS_PlayerController.h"

ALS_PlayerController::ALS_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALS_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ALS_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

