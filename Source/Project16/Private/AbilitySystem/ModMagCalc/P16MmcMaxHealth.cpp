// Copyright © 2025, Ivan Kinder

#include "Public/AbilitySystem/ModMagCalc/P16MmcMaxHealth.h"

#include "AbilitySystem/P16AttributeSet.h"
#include "Interface/P16CombatInterface.h"

UP16MmcMaxHealth::UP16MmcMaxHealth()
{
	VigorDefinition.AttributeToCapture = UP16AttributeSet::GetVigorAttribute();
	VigorDefinition.AttributeSource    = EGameplayEffectAttributeCaptureSource::Target;
	VigorDefinition.bSnapshot          = false;

	RelevantAttributesToCapture.Add(VigorDefinition);
}

float UP16MmcMaxHealth::CalculateBaseMagnitude_Implementation(const FGameplayEffectSpec& Spec) const
{
	// Gather tags from source and target.
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Vigor = 0.f;
	GetCapturedAttributeMagnitude(VigorDefinition, Spec, EvaluateParameters, Vigor);
	Vigor = FMath::Max(0.f, Vigor);

	const TScriptInterface<IP16CombatInterface> CombatInterface = Spec.GetContext().GetSourceObject();

	constexpr float BaseValue        = 80.f;
	constexpr float VigorCoefficient = 2.5f;
	constexpr float LevelCoefficient = 10.f;
	const int32     PlayerLevel      = CombatInterface ? CombatInterface->GetPlayerLevel() : 0;

	return BaseValue
		+ VigorCoefficient * Vigor
		+ LevelCoefficient * PlayerLevel;
}
