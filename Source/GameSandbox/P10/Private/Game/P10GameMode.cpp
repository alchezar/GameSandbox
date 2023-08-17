// Copyright (C) 2023, IKinder

#include "P10/Public/Game/P10GameMode.h"

#include "P10/Public/Player/P10Character.h"
#include "P10/Public/UI/P10HUD.h"

AP10GameMode::AP10GameMode()
{
	DefaultPawnClass = AP10Character::StaticClass();
	HUDClass = AP10HUD::StaticClass();
}
