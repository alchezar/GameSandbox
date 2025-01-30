// Copyright © 2025, Ivan Kinder

#include "Gameplay/Effects/P15DeathEffect.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UP15DeathEffect::UP15DeathEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Infinite;

	if (UTargetTagsGameplayEffectComponent* GrantedTagsComp = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>("GrantedTagsComponent"))
	{
		FInheritedTagContainer TagContainer;
		TagContainer.Added.AddTag(FGameplayTag::RequestGameplayTag("p15.char.death"));

		GrantedTagsComp->SetAndApplyTargetTagChanges(TagContainer);
		GEComponents.Add(GrantedTagsComp);
	}
}
