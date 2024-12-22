// Copyright (C) 2023, IKinder

#include "TG_AiController.h"

ATG_AiController::ATG_AiController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATG_AiController::BeginPlay()
{
	Super::BeginPlay();
}

void ATG_AiController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

