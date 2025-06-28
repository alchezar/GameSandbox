// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/P17AttributeSet.h"

#include "GameplayEffectExtension.h"

UP17AttributeSet::UP17AttributeSet()
{}

void UP17AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	if (Data.EvaluatedData.Attribute == GetHealthCurrentAttribute())
	{
		const float NewHealthCurrent = FMath::Clamp(GetHealthCurrent(), 0.f, GetHealthMax());
		SetHealthCurrent(NewHealthCurrent);
	}
	if (Data.EvaluatedData.Attribute == GetRageCurrentAttribute())
	{
		const float NewRageCurrent = FMath::Clamp(GetRageCurrent(), 0.f, GetRageMax());
		SetRageCurrent(NewRageCurrent);
	}
	if (Data.EvaluatedData.Attribute == GetDamageTakenAttribute())
	{
		const float OldHealth = GetHealthCurrent();
		const float Damage = GetDamageTaken();
		const float NewHealth = FMath::Clamp(OldHealth - Damage, 0.f, GetHealthMax());
		SetHealthCurrent(NewHealth);

		// TODO: Notify UI
		// TODO: Handle death
	}
}
