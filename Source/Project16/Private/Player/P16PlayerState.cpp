// Copyright © 2025, Ivan Kinder

#include "Player/P16PlayerState.h"

#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AttributeSet.h"

AP16PlayerState::AP16PlayerState()
{
	SetNetUpdateFrequency(100.f);

	AbilitySystemComponent = CreateDefaultSubobject<UP16AbilitySystemComponent>("AbilitySystemComponent");
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UP16AttributeSet>("AttributeSetComponent");
}

UAbilitySystemComponent* AP16PlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent.Get();
}

UAttributeSet* AP16PlayerState::GetAttributeSet() const
{
	return AttributeSet.Get();
}
