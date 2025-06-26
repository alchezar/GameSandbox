// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/P17HeroGameplayAbility.h"

#include "Component/Combat/P17CombatHeroComponent.h"
#include "Public/Character/P17CharacterHero.h"
#include "Public/Controller/P17ControllerHero.h"

AP17CharacterHero* UP17HeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedHeroCharacter.IsValid())
	{
		CachedHeroCharacter = Cast<AP17CharacterHero>(CurrentActorInfo->AvatarActor);
	}

	return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

AP17ControllerHero* UP17HeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedHeroController.IsValid())
	{
		CachedHeroController = Cast<AP17ControllerHero>(CurrentActorInfo->PlayerController);
	}

	return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

UP17CombatHeroComponent* UP17HeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	if (!CachedHeroCombatComponent.IsValid())
	{
		CachedHeroCombatComponent = Cast<UP17CombatHeroComponent>(GetCombatPawnComponentFromActorInfo());
	}

	return CachedHeroCombatComponent.IsValid() ? CachedHeroCombatComponent.Get() : nullptr;
}
