// Copyright (C) 2023, IKinder

#include "P8/Public/Player/P8PlayerControllerMainMenu.h"

AP8PlayerControllerMainMenu::AP8PlayerControllerMainMenu()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AP8PlayerControllerMainMenu::BeginPlay()
{
	Super::BeginPlay();
	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}