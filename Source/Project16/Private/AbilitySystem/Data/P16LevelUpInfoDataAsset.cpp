// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16LevelUpInfoDataAsset.h"

int32 UP16LevelUpInfoDataAsset::FindLevelForXP(const int32 XP)
{
	const int32 Index = LevelUpInfos.IndexOfByPredicate([XP](const FP16LevelUpInfo Info) -> bool
	{
		return Info.LevelUpRequirement > XP;
	});

	// Index & Level: 0    1     2     3     4
	// Required XP  : 0    300   600   900   1200
	// Current XP   :               666

	return Index == INDEX_NONE ? 1 : Index;
}
