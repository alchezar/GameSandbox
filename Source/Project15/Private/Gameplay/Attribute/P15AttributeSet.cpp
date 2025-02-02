// Copyright © 2025, Ivan Kinder

#include "Gameplay/Attribute/P15AttributeSet.h"

#include "GameplayEffectExtension.h"

///
/// @brief Update the attribute and broadcast the change if it name matches.
///
#define APPLY_ATTRIBUTE_CHANGE(AttributeName)                                        \
if (Data.EvaluatedData.Attribute.GetUProperty()                                      \
	== FindFieldChecked<FProperty>(StaticClass(), FName(TEXT(#AttributeName))))      \
{                                                                                    \
	AttributeName.SetBaseValue(FMath::Max(0.f, AttributeName.GetBaseValue()));       \
	AttributeName.SetCurrentValue(FMath::Max(0.f, AttributeName.GetCurrentValue())); \
	On##AttributeName##Changed.Broadcast(Get##AttributeName##Percentage());          \
}

UP15AttributeSet::UP15AttributeSet()
{}

void UP15AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
#if 0
	const FProperty* ModifiedProp = Data.EvaluatedData.Attribute.GetUProperty();
	const FProperty* ExpectedProp = FindFieldChecked<FProperty>(StaticClass(), GET_MEMBER_NAME_CHECKED(UP15AttributeSet, Health));
	if (ModifiedProp == ExpectedProp)
	{
		Health.SetBaseValue(FMath::Max(0.f, Health.GetBaseValue()));
		Health.SetCurrentValue(FMath::Max(0.f, Health.GetCurrentValue()));

		OnHealthChanged.Broadcast(GetHealthPercentage());
	}
#endif

	APPLY_ATTRIBUTE_CHANGE(Health)
	APPLY_ATTRIBUTE_CHANGE(Mana)
	APPLY_ATTRIBUTE_CHANGE(Strength)
}

float UP15AttributeSet::GetHealthPercentage() const
{
	return GetPercentage(Health, MaxHealth);
}

float UP15AttributeSet::GetManaPercentage() const
{
	return GetPercentage(Mana, MaxMana);
}

float UP15AttributeSet::GetStrengthPercentage() const
{
	return GetPercentage(Strength, MaxStrength);
}

float UP15AttributeSet::GetPercentage(const FGameplayAttributeData& Attribute, const FGameplayAttributeData& MaxAttribute) const
{
	return FMath::Max(0.f, Attribute.GetCurrentValue() / MaxAttribute.GetCurrentValue());
}
