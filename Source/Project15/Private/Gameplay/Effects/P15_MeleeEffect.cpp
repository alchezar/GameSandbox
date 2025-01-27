// Copyright © 2025, Ivan Kinder

#include "Gameplay/Effects/P15_MeleeEffect.h"

#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UP15_MeleeEffect::UP15_MeleeEffect()
{
	// Cooldown duration.
	DurationPolicy    = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat{1.f};

	// Add tag for cooldown.
	if (const TObjectPtr<UTargetTagsGameplayEffectComponent> GrantTagsToTargetActor = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>("GrantTagsToTargetActorComponent"))
	{
		FInheritedTagContainer InheritedTagContainer;
		InheritedTagContainer.Added.AddTag(FGameplayTag::RequestGameplayTag("p15.melee.cooldown"));

		GrantTagsToTargetActor->SetAndApplyTargetTagChanges(InheritedTagContainer);
		GEComponents.Add(GrantTagsToTargetActor);
	}
}
