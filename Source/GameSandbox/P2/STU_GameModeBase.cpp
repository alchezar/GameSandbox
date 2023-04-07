// Copyright (C) 2023, IKinder

#include "STU_GameModeBase.h"
#include "Player/STU_BaseCharacter.h"
#include "Player/STU_PlayerController.h"
#include "GameSandbox/P2/UI/STU_GameHUD.h"

ASTU_GameModeBase::ASTU_GameModeBase()
{
	DefaultPawnClass = ASTU_BaseCharacter::StaticClass();
	PlayerControllerClass = ASTU_PlayerController::StaticClass();
	HUDClass = ASTU_GameHUD::StaticClass();
}