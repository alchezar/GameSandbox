// Copyright (C) 2023, IKinder

#include "Player/TG_PlayerController.h"

ATG_PlayerController::ATG_PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
	bAutoManageActiveCameraTarget = false;
}

void ATG_PlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void ATG_PlayerController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

