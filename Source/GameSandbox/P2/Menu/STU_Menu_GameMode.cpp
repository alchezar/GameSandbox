// Copyright (C) 2023, IKinder

#include "STU_Menu_GameMode.h"
#include "Menu/STU_Menu_PlayerController.h"
#include "Menu/UI/STU_Menu_HUD.h"

ASTU_Menu_GameMode::ASTU_Menu_GameMode()
{
	PlayerControllerClass = ASTU_Menu_PlayerController::StaticClass();
	HUDClass = ASTU_Menu_HUD::StaticClass();
}