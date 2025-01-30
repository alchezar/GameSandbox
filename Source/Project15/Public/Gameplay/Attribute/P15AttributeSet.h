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

	/* ------------------------------ Fields ------------------------------- */
public:
	FP15OnHealthChangedSignature OnHealthChanged;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData Health = {100.f};
	ATTRIBUTE_ACCESSORS(UP15AttributeSet, Health);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayAttributeData MaxHealth = {100.f};
};
