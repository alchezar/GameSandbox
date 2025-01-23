// Copyright (C) 2023, IKinder

#include "Game/P11SavePlayerInfo.h"

UP11SavePlayerInfo::UP11SavePlayerInfo()
{
	PlayerName = FText::FromString("");
	PlayerSide = EP11PlayerSide::None;
	bShowFPS   = false;
}
