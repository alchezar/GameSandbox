// Copyright (C) 2023, IKinder

#include "P9/Public/Game/P9GameMode.h"

#include "P9/Public/Character/P9PunchCharacter.h"
#include "P9/Public/UI/P9HUD.h"

AP9GameMode::AP9GameMode()
{
	if (DefaultHUDClass)
	{
		HUDClass = DefaultHUDClass;
	}
	if (DefaultCharacterClass)
	{
		DefaultPawnClass = DefaultCharacterClass;
	}
}
