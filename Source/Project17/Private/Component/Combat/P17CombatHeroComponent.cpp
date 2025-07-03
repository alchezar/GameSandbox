// Copyright © 2025, Ivan Kinder

#include "Component/Combat/P17CombatHeroComponent.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Item/Weapon/P17WeaponHero.h"
#include "Util/P17GameplayTags.h"

void UP17CombatHeroComponent::OnHitTargetActorCallback(AActor* HitActor)
{
	RETURN_IF(!HitActor || OverlappedActors.Contains(HitActor),)
	OverlappedActors.Add(HitActor);

	SendGameplayEventToActor(HitActor);
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), P17::Tags::Player_Event_Hit_Pause, {});
}

void UP17CombatHeroComponent::OnWeaponPulledFromActorCallback(AActor* PulledActor)
{
	RETURN_IF(!PulledActor || !OverlappedActors.Contains(PulledActor),)
	OverlappedActors.Remove(PulledActor);

	// UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(GetOwningPawn(), P17::Tags::Player_Event_Hit_Pause, {});
}

AP17WeaponHero* UP17CombatHeroComponent::GetHeroCarriedWeaponBy(const FGameplayTag InWeaponTag) const
{
	return Cast<AP17WeaponHero>(GetCarriedWeaponBy(InWeaponTag));
}

AP17WeaponHero* UP17CombatHeroComponent::GetHeroEquippedWeapon() const
{
	return Cast<AP17WeaponHero>(GetEquippedWeapon());
}

float UP17CombatHeroComponent::GetHeroEquippedWeaponDamage(const float InLevel) const
{
	const AP17WeaponHero* EquippedWeapon = GetHeroEquippedWeapon();
	WARN_RETURN_IF(!EquippedWeapon, 0.f)

	return EquippedWeapon->GetHeroWeaponData().BaseDamage.GetValueAtLevel(InLevel);
}
