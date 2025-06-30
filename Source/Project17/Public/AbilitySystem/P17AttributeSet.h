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

class UP17UIHeroComponent;
class UP17UIPawnComponent;
class IP17UIInterface;

UCLASS()
class PROJECT17_API UP17AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP17AttributeSet();

	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

private:
	_NODISCARD
	TWeakInterfacePtr<IP17UIInterface> GetCachedUIInterface(AActor* InActor);

	void NotifyRageChange(const FGameplayEffectModCallbackData& Data);
	void NotifyHealthChange(const FGameplayEffectModCallbackData& Data);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
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

	/// @par Meta attributes ---------------------------------------------------

	UPROPERTY(BlueprintReadOnly, Category = "C++ | Attribute | Damage")
	FGameplayAttributeData DamageTaken = {};
	ATTRIBUTE_ACCESSORS(DamageTaken)

private:
	TWeakInterfacePtr<IP17UIInterface> CachedUIInterface = nullptr;
};
