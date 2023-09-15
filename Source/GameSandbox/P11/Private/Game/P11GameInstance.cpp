// Copyright (C) 2023, IKinder

#include "P11/Public/Game/P11GameInstance.h"

#include "P11/Public/Game/P11SavePlayerInfo.h"

void UP11GameInstance::Init()
{
	Super::Init();

	SavePlayerInfo = NewObject<UP11SavePlayerInfo>(GetTransientPackage(), UP11SavePlayerInfo::StaticClass());
}
