// Copyright © 2025, Ivan Kinder

#include "Gameplay/Effects/P15MeleeEffect.h"

#include "Gameplay/Attribute/P15AttributeSet.h"
#include "GameplayEffectComponents/TargetTagsGameplayEffectComponent.h"

UP15MeleeEffect::UP15MeleeEffect()
{
	// Cooldown duration.
	DurationPolicy    = EGameplayEffectDurationType::HasDuration;
	DurationMagnitude = FScalableFloat{1.f};

	// Add tag for cooldown.
	if (UTargetTagsGameplayEffectComponent* GrantTagsToTargetActor = CreateDefaultSubobject<UTargetTagsGameplayEffectComponent>("GrantTagsToTargetActorComponent"))
	{
		FInheritedTagContainer InheritedTagContainer;
		InheritedTagContainer.Added.AddTag(FGameplayTag::RequestGameplayTag("p15.melee.cooldown"));

		GrantTagsToTargetActor->SetAndApplyTargetTagChanges(InheritedTagContainer);
		GEComponents.Add(GrantTagsToTargetActor);
	}
}

UP15MeleeDamageEffect::UP15MeleeDamageEffect()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;

	// Set the target attribute and the damage amount.
	FGameplayModifierInfo HealthModifier;
	HealthModifier.Attribute         = UP15AttributeSet::GetHealthAttribute();
	HealthModifier.ModifierOp        = EGameplayModOp::Type::Additive;
	HealthModifier.ModifierMagnitude = FScalableFloat(-10.f);

	Modifiers.Add(HealthModifier);
}
