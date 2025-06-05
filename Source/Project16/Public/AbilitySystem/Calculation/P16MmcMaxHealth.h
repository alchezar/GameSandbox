// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "P16MmcMaxHealth.generated.h"

UCLASS()
class PROJECT16_API UP16MmcMaxHealth : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP16MmcMaxHealth();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
private:
	FGameplayEffectAttributeCaptureDefinition VigorDefinition;
};
