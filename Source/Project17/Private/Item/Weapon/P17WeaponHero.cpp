// Copyright © 2025, Ivan Kinder

#include "Item/Weapon/P17WeaponHero.h"

AP17WeaponHero::AP17WeaponHero()
{}

TArray<FGameplayAbilitySpecHandle>& AP17WeaponHero::GetGrantedAbilitySpecHandles()
{
	return GrantedAbilitySpecHandles;
}

void AP17WeaponHero::SetGrantedAbilitySpecHandles(TArray<FGameplayAbilitySpecHandle>& InGrantedAbilitySpecHandles)
{
	GrantedAbilitySpecHandles = MoveTemp(InGrantedAbilitySpecHandles);
}
