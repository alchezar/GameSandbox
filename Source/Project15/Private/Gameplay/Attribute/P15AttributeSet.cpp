// Copyright © 2025, Ivan Kinder

#include "Gameplay/Attribute/P15AttributeSet.h"

#include "GameplayEffectExtension.h"

UP15AttributeSet::UP15AttributeSet()
{}

void UP15AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	const FProperty* ModifiedProp = Data.EvaluatedData.Attribute.GetUProperty();
	const FProperty* ExpectedProp = FindFieldChecked<FProperty>(StaticClass(), GET_MEMBER_NAME_CHECKED(UP15AttributeSet, Health));
	if (ModifiedProp == ExpectedProp)
	{
		Health.SetBaseValue(FMath::Max(0.f, Health.GetBaseValue()));
		Health.SetCurrentValue(FMath::Max(0.f, Health.GetCurrentValue()));

		OnHealthChanged.Broadcast(GetHealthPercentage());
	}
}

float UP15AttributeSet::GetHealthPercentage() const
{
	return FMath::Max(0.f, Health.GetCurrentValue() / MaxHealth.GetCurrentValue());
}
