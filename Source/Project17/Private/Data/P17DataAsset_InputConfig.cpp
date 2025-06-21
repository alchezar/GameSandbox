// Copyright © 2025, Ivan Kinder

#include "Data/P17DataAsset_InputConfig.h"

UInputAction* UP17DataAsset_InputConfig::FindNativeInputAction(const FGameplayTag& InInputTag) const
{
	const FP17InputActionConfig* Result = NativeInputActions.FindByPredicate(
		[InInputTag](const FP17InputActionConfig& ActionConfig) -> bool
		{
			return ActionConfig.Tag == InInputTag;
		});

	return (Result && Result->Action) ? Result->Action : nullptr;
}
