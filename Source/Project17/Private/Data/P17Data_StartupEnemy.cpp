// Copyright © 2025, Ivan Kinder

#include "Data/P17Data_StartupEnemy.h"

#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Base/P17EnemyGameplayAbility.h"
#include "Project17/Project17.h"

void UP17Data_StartupEnemy::GiveToAbilitySystemComponent(UP17AbilitySystemComponent* InASC, const int32 InLevel)
{
	Super::GiveToAbilitySystemComponent(InASC, InLevel);

	for (const TSubclassOf<UP17EnemyGameplayAbility>& Ability : EnemyAbilities)
	{
		WARN_CONTINUE_IF(!IsValid(Ability))
		InASC->GiveAbility(FGameplayAbilitySpec {Ability, InLevel, INDEX_NONE, InASC->GetAvatarActor()});
	}
}
