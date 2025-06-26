// Copyright © 2025, Ivan Kinder

#include "Data/P17Data_StartupHero.h"

#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/P17HeroGameplayAbility.h"
#include "Project17/Project17.h"

void UP17Data_StartupHero::GiveToAbilitySystemComponent(UP17AbilitySystemComponent* InASC, const int32 InLevel)
{
	Super::GiveToAbilitySystemComponent(InASC, InLevel);

	for (const FP17HeroAbilitySet& AbilitySet : HeroStartupAbilitySets)
	{
		CONTINUE_IF(!AbilitySet.IsValid())

		FGameplayAbilitySpec AbilitySpec {AbilitySet.Ability};
		AbilitySpec.SourceObject = InASC->GetAvatarActor();
		AbilitySpec.Level = InLevel;
		AbilitySpec.GetDynamicSpecSourceTags().AddTag(AbilitySet.InputTag);

		InASC->GiveAbility(AbilitySpec);
	}
}
