// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16AttributeInfoDataAsset.h"

#include "Util/P16Log.h"

FP16AttributeInfo UP16AttributeInfoDataAsset::FindAttributeInfo(const FGameplayTag& InTag, const UAttributeSet* InAttributeSet, const bool bLogNotFound) const
{
	const FP16AttributeInfo* FoundInfo = AttributeInfos.FindByPredicate([InTag](const FP16AttributeInfo& Info) -> bool
	{
		return Info.Tag.MatchesTagExact(InTag);
	});

	FP16AttributeInfo Result = {};
	if (FoundInfo)
	{
		Result       = *FoundInfo;
		Result.Value = Result.Attribute.GetNumericValue(InAttributeSet);
	}
	else if (bLogNotFound)
	{
		UE_LOG(LogP16, Error, L"Can't find Info for AttributeTag[%s] in AttributeInfo [%s].", *InTag.ToString(), *GetNameSafe(this));
	}

	return Result;
}
