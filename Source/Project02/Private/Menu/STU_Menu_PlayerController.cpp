// Copyright (C) 2023, IKinder

#include "Menu/STU_Menu_PlayerController.h"

void ASTU_Menu_PlayerController::BeginPlay()
{
	Super::BeginPlay();

	bShowMouseCursor = true;
	SetInputMode(FInputModeUIOnly());
}