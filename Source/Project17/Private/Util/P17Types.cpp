// Copyright © 2025, Ivan Kinder

#include "Util/P17Types.h"

#include "AbilitySystem/Abilities/Base/P17HeroGameplayAbility.h"

bool FP17HeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && Ability;
}
