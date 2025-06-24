// Copyright © 2025, Ivan Kinder

#include "Util/P17Types.h"

#include "AbilitySystem/Abilities/P17GameplayAbility.h"

bool FP17HeroAbilitySet::IsValid() const
{
	return InputTag.IsValid() && Ability;
}
