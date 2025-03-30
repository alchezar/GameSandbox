// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16LevelUpInfoDataAsset.h"

int32 UP16LevelUpInfoDataAsset::FindLevelForXP(const int32 XP) const
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

float UP16LevelUpInfoDataAsset::GetLevelPercentage(const int32 XP) const
{
	const int32 Index = FindLevelForXP(XP);

	const int32 NextLevelUpRequirement = LevelUpInfos[Index].LevelUpRequirement;
	const int32 PrevLevelUpRequirement = LevelUpInfos[Index - 1].LevelUpRequirement;

	const int32 RequirementDelta = NextLevelUpRequirement - PrevLevelUpRequirement;
	const int32 CurrentDelta     = XP - PrevLevelUpRequirement;

	return static_cast<float>(CurrentDelta) / static_cast<float>(RequirementDelta);
}
