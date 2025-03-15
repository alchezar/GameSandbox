// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AbilitySystemGlobals.h"

#include "AbilitySystem/P16GameplayEffectContext.h"

FGameplayEffectContext* UP16AbilitySystemGlobals::AllocGameplayEffectContext() const
{
	return new FP16GameplayEffectContext{};
}
