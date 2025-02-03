// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "AttributeSet.h"
#include "Project15.h"
#include "Utils/P15Utils.h"
#include "P15AttributeSet.generated.h"

UCLASS()
class PROJECT15_API UP15AttributeSet : public UAttributeSet
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15AttributeSet();

protected:
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;

	/* ------------------------------- This -------------------------------- */
public:
	_NODISCARD float GetHealthPercentage() const;
	_NODISCARD float GetManaPercentage() const;
	_NODISCARD float GetStrengthPercentage() const;

private:
	_NODISCARD float GetPercentage(const FGameplayAttributeData& Attribute, const FGameplayAttributeData& MaxAttribute) const;

	/* ------------------------------ Fields ------------------------------- */
public:
	FP15OnHealthChangedSignature OnHealthChanged;
	FP15OnHealthChangedSignature OnManaChanged;
	FP15OnHealthChangedSignature OnStrengthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData MaxHealth = {100.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData Health = {100.f};
	ATTRIBUTE_ACCESSORS(UP15AttributeSet, Health);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData MaxMana = {100.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData Mana = {100.f};
	ATTRIBUTE_ACCESSORS(UP15AttributeSet, Mana);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData MaxStrength = {100.f};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData Strength = {100.f};
	ATTRIBUTE_ACCESSORS(UP15AttributeSet, Strength);
};
