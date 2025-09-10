// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Calculation/P17ExecCalcDamage.h"

#include "Project17.h"
#include "AbilitySystem/P17AttributeSet.h"
#include "Util/P17GameplayTags.h"

/// @section FP17DamageCapture =================================================

struct FP17DamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(PowerAttack)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PowerDefence)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FP17DamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP17AttributeSet, PowerAttack, Source, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP17AttributeSet, PowerDefence, Target, false)
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP17AttributeSet, DamageTaken, Target, false)
	}

	static const FP17DamageCapture& Get()
	{
		static FP17DamageCapture DamageCapture;
		return DamageCapture;
	}
};

/// @section UP17ExecCalcDamage ================================================

UP17ExecCalcDamage::UP17ExecCalcDamage()
{
	RelevantAttributesToCapture.Add(FP17DamageCapture::Get().PowerAttackDef);
	RelevantAttributesToCapture.Add(FP17DamageCapture::Get().PowerDefenceDef);
	RelevantAttributesToCapture.Add(FP17DamageCapture::Get().DamageTakenDef);
}

void UP17ExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& EffectSpec = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle EffectContext = EffectSpec.GetContext();

	FAggregatorEvaluateParameters EvaluateParams {};
	EvaluateParams.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParams.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FP17DamageCapture::Get().PowerAttackDef, EvaluateParams, SourceAttackPower);
	float TargetDefencePower = 1.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(FP17DamageCapture::Get().PowerDefenceDef, EvaluateParams, TargetDefencePower);

	float BaseDamage = EffectSpec.GetSetByCallerMagnitude(P17::Tags::Shared_SetByCaller_Damage_Base, false);
	const float ComboLight = EffectSpec.GetSetByCallerMagnitude(P17::Tags::Player_SetByCaller_AttackType_Light, false);
	const float ComboHeavy = EffectSpec.GetSetByCallerMagnitude(P17::Tags::Player_SetByCaller_AttackType_Heavy, false);

	if (ComboLight > 0.f)
	{
		constexpr float ComboLightScale = 0.05f;
		BaseDamage *= (ComboLight * ComboLightScale);
	}
	if (ComboHeavy > 0.f)
	{
		constexpr float ComboHeavyScale = 0.15f;
		BaseDamage *= (ComboHeavy * ComboHeavyScale);
	}

	const float FinalDamage = BaseDamage * SourceAttackPower / FMath::Max(TargetDefencePower, 1.f);
	RETURN_IF(FinalDamage <= 0.f,)

	const FGameplayModifierEvaluatedData EvaluatedData = {FP17DamageCapture::Get().DamageTakenProperty, EGameplayModOp::Override, FinalDamage};
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}
