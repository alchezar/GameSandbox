// Copyright © 2025, Ivan Kinder

#include "Project16/Public/AbilitySystem/Calculation/P16MmcMaxMana.h"

#include "AbilitySystem/P16AttributeSet.h"
#include "Interface/P16CombatInterface.h"

UP16MmcMaxMana::UP16MmcMaxMana()
{
	IntelligenceDefinition.AttributeToCapture = UP16AttributeSet::GetIntelligenceAttribute();
	IntelligenceDefinition.AttributeSource    = EGameplayEffectAttributeCaptureSource::Target;
	IntelligenceDefinition.bSnapshot          = false;

	RelevantAttributesToCapture.Add(IntelligenceDefinition);
}

float UP16MmcMaxMana::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target.
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Intelligence = 0.f;
	GetCapturedAttributeMagnitude(IntelligenceDefinition, Spec, EvaluateParameters, Intelligence);
	Intelligence = FMath::Max(0.f, Intelligence);

	const TScriptInterface<IP16CombatInterface> CombatInterface = Spec.GetContext().GetSourceObject();

	constexpr float BaseValue        = 50.f;
	constexpr float IntelCoefficient = 2.5f;
	constexpr float LevelCoefficient = 15.f;
	const int32     PlayerLevel      = CombatInterface ? CombatInterface->GetPlayerLevel() : 0;

	return BaseValue
		+ IntelCoefficient * Intelligence
		+ LevelCoefficient * PlayerLevel;
}
