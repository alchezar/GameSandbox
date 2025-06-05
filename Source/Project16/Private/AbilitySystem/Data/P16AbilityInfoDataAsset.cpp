// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16AbilityInfoDataAsset.h"

#include "Util/P16Log.h"

FP16AbilityInfo UP16AbilityInfoDataAsset::FindAbilityInfo(const FGameplayTag& InAbilityTag, const bool bLogNotFound) const
{
	const FP16AbilityInfo* FoundInfo = AbilityInfos.FindByPredicate([InAbilityTag](const FP16AbilityInfo& Info) -> bool
	{
		return Info.AbilityTag.MatchesTagExact(InAbilityTag);
	});

	FP16AbilityInfo Result = {};
	if (FoundInfo)
	{
		Result = *FoundInfo;
	}
	else if (bLogNotFound)
	{
		UE_LOG(LogP16, Error, L"Can't find Info for AbilityTag[%s] in AbilityInfo [%s].", *InAbilityTag.ToString(), *GetNameSafe(this));
	}

	return Result;
}
