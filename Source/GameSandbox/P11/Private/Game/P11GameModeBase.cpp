// Copyright (C) 2023, IKinder

#include "P11/Public/Game/P11GameModeBase.h"

#include "P11/Public/Player/P11Character.h"
#include "P11/Public/UI/P11HUD.h"

AP11GameModeBase::AP11GameModeBase()
{
	DefaultPawnClass = AP11Character::StaticClass();
	HUDClass = AP11HUD::StaticClass();
}
