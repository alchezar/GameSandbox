// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayModMagnitudeCalculation.h"
#include "P16MmcMaxMana.generated.h"

UCLASS()
class PROJECT16_API UP16MmcMaxMana : public UGameplayModMagnitudeCalculation
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP16MmcMaxMana();
	virtual float CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const override;

	/* ------------------------------ Fields ------------------------------- */
private:
	FGameplayEffectAttributeCaptureDefinition IntelligenceDefinition;
};
