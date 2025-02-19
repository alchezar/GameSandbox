// Copyright © 2025, Ivan Kinder

///
/// @brief Update the attribute and broadcast the change if it name matches.
///
#define APPLY_ATTRIBUTE_CHANGE(AttributeName)                                    \
if (Data.EvaluatedData.Attribute.GetUProperty()                                  \
	== FindFieldChecked<FProperty>(StaticClass(), FName(TEXT(#AttributeName))))  \
{                                                                                \
	AttributeName.SetBaseValue(FMath::Clamp(                                     \
		AttributeName.GetBaseValue(),                                            \
		0.f,                                                                     \
		Max##AttributeName.GetBaseValue()));                                     \
	AttributeName.SetCurrentValue(FMath::Clamp(                                  \
		AttributeName.GetCurrentValue(),                                         \
		0.f,                                                                     \
		Max##AttributeName.GetCurrentValue()));                                  \
	On##AttributeName##Changed.Broadcast(Get##AttributeName##Percentage());      \
}

#include "Gameplay/Attribute/P15AttributeSet.h"

#include "GameplayEffectExtension.h"

UP15AttributeSet::UP15AttributeSet()
{}

void UP15AttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
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

float UP15AttributeSet::GetAllPercentage() const
{
	return GetHealthPercentage() * GetManaPercentage() * GetStrengthPercentage();
}

float UP15AttributeSet::GetPercentage(const FGameplayAttributeData& Attribute, const FGameplayAttributeData& MaxAttribute) const
{
	return FMath::Max(0.f, Attribute.GetCurrentValue() / MaxAttribute.GetCurrentValue());
}
