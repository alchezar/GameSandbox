// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Calculation/P16ExecCalcDamage.h"

#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "AbilitySystem/Data/P16CharacterClassInfoDataAsset.h"
#include "Interface/P16CombatInterface.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

/// ----------------------------------------------------------------------------
/// @class FP16DamageStatics
/// ----------------------------------------------------------------------------

struct FP16DamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)

	FP16DamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, Armor, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, ArmorPenetration, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, BlockChance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, CriticalHitChance, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, CriticalHitDamage, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, CriticalHitResistance, Target, false);
	}

	static const FP16DamageStatics& Get()
	{
		static FP16DamageStatics Statics;
		return Statics;
	}
};

/// ----------------------------------------------------------------------------
/// @class FP16ExecutionData
/// ----------------------------------------------------------------------------

FP16ExecutionData::FP16ExecutionData(const FGameplayEffectCustomExecutionParameters& ExecutionParams)
{
	SourceAbilitySystemComponent = ExecutionParams.GetSourceAbilitySystemComponent();
	TargetAbilitySystemComponent = ExecutionParams.GetTargetAbilitySystemComponent();

	SourceAvatar = SourceAbilitySystemComponent ? SourceAbilitySystemComponent->GetAvatarActor() : nullptr;
	TargetAvatar = TargetAbilitySystemComponent ? TargetAbilitySystemComponent->GetAvatarActor() : nullptr;

	SourceCombat = SourceAvatar;
	TargetCombat = TargetAvatar;
}

/// ----------------------------------------------------------------------------
/// @class UP16ExecCalcDamage
/// ----------------------------------------------------------------------------

UP16ExecCalcDamage::UP16ExecCalcDamage()
{
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().ArmorDef);
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().ArmorPenetrationDef);
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().BlockChanceDef);
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().CriticalHitChanceDef);
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().CriticalHitDamageDef);
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().CriticalHitResistanceDef);
}

void UP16ExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Get damage set by Caller magnitude.
	const FGameplayEffectSpec Spec   = ExecutionParams.GetOwningSpec();
	float                     Damage = Spec.GetSetByCallerMagnitude(FGSGameplayTagsSingleton::Get().P16Tags.Damage);

	// Affect damage by modifiers.
	const FP16ExecutionData ExecutionData{ExecutionParams};

	AffectBlockChance(ExecutionParams, Damage);
	AffectArmorAndPenetration(ExecutionParams, ExecutionData, Damage);
	AffectCriticalHit(ExecutionParams, ExecutionData, Damage);

	// Set incoming damage attribute.
	const FGameplayModifierEvaluatedData EvaluatedData = {UP16AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage};
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UP16ExecCalcDamage::AffectBlockChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, float& OutDamage) const
{
	const float TargetBlockChance = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().BlockChanceDef);
	EARLY_RETURN_IF(!HasChance(TargetBlockChance))

	OutDamage /= 2.f;
}

void UP16ExecCalcDamage::AffectArmorAndPenetration(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FP16ExecutionData& ExecutionData, float& OutDamage) const
{
	const AActor*                               SourceAvatar = ExecutionData.SourceAvatar;
	const AActor*                               TargetAvatar = ExecutionData.TargetAvatar;
	const TScriptInterface<IP16CombatInterface> SourceCombat = ExecutionData.SourceCombat;
	const TScriptInterface<IP16CombatInterface> TargetCombat = ExecutionData.TargetCombat;

	const float ArmorPenetrationCoefficient = GetCoefficient(SourceAvatar, "ArmorPenetration", SourceCombat->GetPlayerLevel());
	const float EffectiveArmorCoefficient   = GetCoefficient(TargetAvatar, "EffectiveArmor", TargetCombat->GetPlayerLevel());

	const float TargetArmor            = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().ArmorDef);
	const float SourceArmorPenetration = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().ArmorPenetrationDef);
	const float EffectiveArmor         = TargetArmor * (1.f - SourceArmorPenetration * ArmorPenetrationCoefficient / 100.f);
	const float FinalCoefficient       = 1.f - EffectiveArmor * EffectiveArmorCoefficient / 100.f;

	OutDamage *= FinalCoefficient;
}

void UP16ExecCalcDamage::AffectCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FP16ExecutionData& ExecutionData, float& OutDamage) const
{
	const AActor*                               TargetAvatar = ExecutionData.TargetAvatar;
	const TScriptInterface<IP16CombatInterface> TargetCombat = ExecutionData.TargetCombat;

	const float CriticalHitChance     = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().CriticalHitChanceDef);
	const float CriticalHitResistance = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().CriticalHitResistanceDef);
	const float CriticalHitDamage     = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().CriticalHitDamageDef);

	const float CriticalHitResistanceCoefficient = GetCoefficient(TargetAvatar, "CriticalHitResistance", TargetCombat->GetPlayerLevel());
	const float EffectiveCriticalHitChance       = CriticalHitChance * (1.f - CriticalHitResistance * CriticalHitResistanceCoefficient / 100.f);
	EARLY_RETURN_IF(!HasChance(EffectiveCriticalHitChance))

	OutDamage *= 2.f;
	OutDamage += CriticalHitDamage;
}

FAggregatorEvaluateParameters UP16ExecCalcDamage::GetEvaluateParameters(const FGameplayEffectCustomExecutionParameters& ExecutionParams) const
{
	const FGameplayEffectSpec Spec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	return EvaluateParameters;
}

bool UP16ExecCalcDamage::HasChance(const float Chance) const
{
	return Chance >= FMath::RandRange(1.f, 100.f);
}

float UP16ExecCalcDamage::GetAttributeMagnitude(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	float Magnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InCaptureDef, GetEvaluateParameters(ExecutionParams), Magnitude);
	Magnitude = FMath::Max(0.f, Magnitude);

	return Magnitude;
}

float UP16ExecCalcDamage::GetCoefficient(const UObject* WorldContextObject, const FName RowName, const float Level, const float DefaultValue) const
{
	const UP16CharacterClassInfoDataAsset* ClassInfo = UP16AbilitySystemLibrary::GetCharacterClassInfo(WorldContextObject);
	EARLY_RETURN_VALUE_IF(!ClassInfo, DefaultValue);
	const FRealCurve* Curve = ClassInfo->DamageCalculationCoefficients->FindCurve(RowName, FString{});
	EARLY_RETURN_VALUE_IF(!Curve, DefaultValue);

	return Curve->Eval(Level, DefaultValue);
}
