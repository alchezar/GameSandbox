// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "P17AttributeSet.generated.h"

#define ATTRIBUTE_ACCESSORS(PropertyName)                  \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ThisClass, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName)               \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName)               \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UCLASS()
class PROJECT17_API UP17AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP17AttributeSet();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(BlueprintReadOnly, Category = "C++ | Attribute | Health")
	FGameplayAttributeData HealthCurrent = {};
	ATTRIBUTE_ACCESSORS(HealthCurrent)

	UPROPERTY(BlueprintReadOnly, Category = "C++ | Attribute | Health")
	FGameplayAttributeData HealthMax = {};
	ATTRIBUTE_ACCESSORS(HealthMax)

	UPROPERTY(BlueprintReadOnly, Category = "C++ | Attribute | Rage")
	FGameplayAttributeData RageCurrent = {};
	ATTRIBUTE_ACCESSORS(RageCurrent)

	UPROPERTY(BlueprintReadOnly, Category = "C++ | Attribute | Rage")
	FGameplayAttributeData RageMax = {};
	ATTRIBUTE_ACCESSORS(RageMax)

	UPROPERTY(BlueprintReadOnly, Category = "C++ | Attribute | Damage")
	FGameplayAttributeData PowerAttack = {};
	ATTRIBUTE_ACCESSORS(PowerAttack)

	UPROPERTY(BlueprintReadOnly, Category = "C++ | Attribute | Damage")
	FGameplayAttributeData PowerDefence = {};
	ATTRIBUTE_ACCESSORS(PowerDefence)
};
