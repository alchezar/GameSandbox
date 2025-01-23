// Copyright (C) 2023, IKinder

#include "Game/P9GameMode.h"

#include "Character/P9PunchCharacter.h"
#include "UI/P9HUD.h"

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
