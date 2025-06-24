// Copyright © 2025, Ivan Kinder

#include "Component/Combat/P17CombatPawnComponent.h"

#include "Item/Weapon/P17WeaponBase.h"
#include "Util/P17DebugHelper.h"

void UP17CombatPawnComponent::RegisterSpawnedWeapon(const FGameplayTag InWeaponTag, AP17WeaponBase* InWeapon, const bool bRegisterAsEquipped)
{
	WARN_RETURN_IF(CarriedWeaponMap.Contains(InWeaponTag),)
	WARN_RETURN_IF(!InWeapon,)

	CarriedWeaponMap.Emplace(InWeaponTag, InWeapon);

	if (bRegisterAsEquipped)
	{
		EquippedWeaponTag = InWeaponTag;
	}
}

AP17WeaponBase* UP17CombatPawnComponent::GetCarriedWeaponBy(const FGameplayTag InWeaponTag) const
{
	WARN_RETURN_IF(!CarriedWeaponMap.Contains(InWeaponTag), nullptr)

	AP17WeaponBase* const* FoundWeapon = CarriedWeaponMap.Find(InWeaponTag);
	WARN_RETURN_IF(!FoundWeapon, nullptr)

	return *FoundWeapon;
}

AP17WeaponBase* UP17CombatPawnComponent::GetEquippedWeapon() const
{
	WARN_RETURN_IF(!EquippedWeaponTag.IsValid(), nullptr)

	return GetCarriedWeaponBy(EquippedWeaponTag);
}

void UP17CombatPawnComponent::SetEquippedWeaponTag(const FGameplayTag InWeaponTag)
{
	EquippedWeaponTag = InWeaponTag;
}
