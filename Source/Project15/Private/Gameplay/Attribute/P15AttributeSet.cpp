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
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, FString::Printf(L"Ohh, I took some damage, now my health is: %f", Health.GetCurrentValue()));
	}
}
