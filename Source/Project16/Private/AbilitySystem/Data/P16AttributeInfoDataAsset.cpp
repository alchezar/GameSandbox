// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16AttributeInfoDataAsset.h"

FP16AttributeInfo UP16AttributeInfoDataAsset::FindAttributeInfo(const FGameplayTag& InTag, const bool bLogNotFound) const
{
	const FP16AttributeInfo* FoundInfo = AttributeInfo.FindByPredicate([InTag](const FP16AttributeInfo& Info) -> bool
	{
		return Info.Tag.MatchesTagExact(InTag);
	});

	FP16AttributeInfo Result = {};
	if (FoundInfo)
	{
		Result = *FoundInfo;
	}
	else if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, L"Can't find Info for AttributeTag[%s] on AttributeInfo [%s].", *InTag.ToString(), *GetNameSafe(this));
	}

	return Result;
}
