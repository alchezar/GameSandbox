// Copyright © 2025, Ivan Kinder

#include "Component/Combat/P17CombatHeroComponent.h"

#include "Item/Weapon/P17WeaponHero.h"

AP17WeaponHero* UP17CombatHeroComponent::GetHeroCarriedWeaponBy(const FGameplayTag InWeaponTag) const
{
	return Cast<AP17WeaponHero>(GetCarriedWeaponBy(InWeaponTag));
}
