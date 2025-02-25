// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P16AttributeSet.h"

#include "AbilitySystemComponent.h"
#include "Net/UnrealNetwork.h"

UP16AttributeSet::UP16AttributeSet()
{}

void UP16AttributeSet::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Health, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHealth, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mana, COND_None, REPNOTIFY_Always)
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMana, COND_None, REPNOTIFY_Always)
}

void UP16AttributeSet::OnRep_Health(const FGameplayAttributeData& OldHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Health, OldHealth)
}

void UP16AttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHealth, OldMaxHealth)
}

void UP16AttributeSet::OnRep_Mana(const FGameplayAttributeData& OldMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mana, OldMana);
}

void UP16AttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldMaxMana) const
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMana, OldMaxMana);
}
