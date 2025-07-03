// Copyright © 2025, Ivan Kinder

#include "Component/Combat/P17CombatPawnComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "Item/Weapon/P17WeaponBase.h"
#include "Util/P17DebugHelper.h"

void UP17CombatPawnComponent::RegisterSpawnedWeapon(const FGameplayTag InWeaponTag, AP17WeaponBase* InWeapon, const bool bRegisterAsEquipped)
{
	WARN_RETURN_IF(CarriedWeaponMap.Contains(InWeaponTag),)
	WARN_RETURN_IF(!InWeapon,)

	CarriedWeaponMap.Emplace(InWeaponTag, InWeapon);

	InWeapon->OnWeaponHitTarget.BindUObject(this, &ThisClass::OnHitTargetActorCallback);
	InWeapon->OnWeaponPulledFromTarget.BindUObject(this, &UP17CombatPawnComponent::OnWeaponPulledFromActorCallback);

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

void UP17CombatPawnComponent::ToggleWeaponCollision(const bool bEnable, const EP17ToggleDamageType InToggleDamageType)
{
	if (InToggleDamageType == EP17ToggleDamageType::EquippedWeapon)
	{
		const AP17WeaponBase* Weapon = GetEquippedWeapon();
		WARN_RETURN_IF(!Weapon,)
		Weapon->ToggleCollision(bEnable);
	}

	if (!bEnable)
	{
		OverlappedActors.Empty();
	}

	// TODO: Handle body collision boxes
}

void UP17CombatPawnComponent::OnHitTargetActorCallback(AActor* HitActor)
{
	// Overloaded by children (<gi> to find them).
}

void UP17CombatPawnComponent::OnWeaponPulledFromActorCallback(AActor* PulledActor)
{
	// Overloaded by children (<gi> to find them).
}

void UP17CombatPawnComponent::SendGameplayEventToActor(AActor* HitActor, const FGameplayTag EventTag) const
{
	FGameplayEventData Payload {};
	Payload.Instigator = GetOwningPawn();
	Payload.Target = HitActor;

	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), EventTag, Payload);
}
