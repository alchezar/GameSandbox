// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/P17EnemyGameplayAbility.h"

#include "Character/P17CharacterEnemy.h"
#include "Component/Combat/P17CombatEnemyComponent.h"

AP17CharacterEnemy* UP17EnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedEnemyCharacter.IsValid())
	{
		CachedEnemyCharacter = Cast<AP17CharacterEnemy>(CurrentActorInfo->AvatarActor);
	}

	return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UP17CombatEnemyComponent* UP17EnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	if (!CachedEnemyCombatComponent.IsValid())
	{
		CachedEnemyCombatComponent = Cast<UP17CombatEnemyComponent>(GetCombatPawnComponentFromActorInfo());
	}

	return CachedEnemyCombatComponent.IsValid() ? CachedEnemyCombatComponent.Get() : nullptr;
}
