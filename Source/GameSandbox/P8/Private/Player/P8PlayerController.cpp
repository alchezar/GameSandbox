// Copyright (C) 2023, IKinder

#include "P8/Public/Player/P8PlayerController.h"

AP8PlayerController::AP8PlayerController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP8PlayerController::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = false;
	SetInputMode(FInputModeGameOnly());
}
