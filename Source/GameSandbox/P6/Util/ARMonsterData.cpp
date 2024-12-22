// Copyright (C) 2023, IKinder

#include "ARMonsterData.h"

FPrimaryAssetId UARMonsterData::GetPrimaryAssetId() const
{
	return FPrimaryAssetId("Monsters", GetFName());
	
}
