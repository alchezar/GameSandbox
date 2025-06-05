// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16InputConfig.h"

const UInputAction* UP16InputConfig::FindAbilityInputAction(const FGameplayTag& InTag, const bool bLogNotFound) const
{
	const FP16InputAction* FoundAction = AbilityInputActions.FindByPredicate([InTag](const FP16InputAction& EachAction) -> bool
	{
		return InTag.MatchesTagExact(EachAction.Tag);
	});

	const UInputAction* Result = nullptr;
	if (FoundAction)
	{
		Result = FoundAction->Action;
	}
	else if (bLogNotFound)
	{
		UE_LOG(LogTemp, Error, L"Can't find AbilityInputAction for InputTag[%s] on InputConfig [%s].", *InTag.ToString(), *GetNameSafe(this));
	}

	return Result;
}
