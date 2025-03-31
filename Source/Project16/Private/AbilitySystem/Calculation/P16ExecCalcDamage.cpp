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
	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArmorPenetration)
	DECLARE_ATTRIBUTE_CAPTUREDEF(BlockChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitChance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitDamage)
	DECLARE_ATTRIBUTE_CAPTUREDEF(CriticalHitResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(ArcaneResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(FireResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(LightningResistance)
	DECLARE_ATTRIBUTE_CAPTUREDEF(PhysicalResistance)

	// TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefinitions was removed from here,
	// because FGSGameplayTagsSingleton is not initialized when FP16DamageStatics constructor is called.

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	FP16DamageStatics()
	{
		// clang-format off
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, Armor,                 Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, ArmorPenetration,      Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, BlockChance,           Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, CriticalHitChance,     Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, CriticalHitDamage,     Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, CriticalHitResistance, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, ArcaneResistance,      Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, FireResistance,        Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, LightningResistance,   Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UP16AttributeSet, PhysicalResistance,    Target, false);
		//clang-format on
	}

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
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
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().ArcaneResistanceDef);
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().FireResistanceDef);
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().LightningResistanceDef);
	RelevantAttributesToCapture.Add(FP16DamageStatics::Get().PhysicalResistanceDef);
}

void UP16ExecCalcDamage::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	// Get damage set by Caller magnitude.
	const FGameplayEffectSpec    Spec          = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

	// Search through all damage types.
	float Damage = 0.f;
	for (const auto& [DamageTag, ResistanceTag] : FGSGameplayTagsSingleton::Get().P16Tags.Maps.DamageTypesToResistances)
	{
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTag, false);
		AffectResistance(ExecutionParams, ContextHandle, ResistanceTag, DamageTypeValue);
		Damage += DamageTypeValue;
	}

	// Affect damage by modifiers.
	const FP16ExecutionData ExecutionData {ExecutionParams};

	AffectBlockChance(ExecutionParams, ContextHandle, Damage);
	AffectArmorAndPenetration(ExecutionParams, ExecutionData, Damage);
	AffectCriticalHit(ExecutionParams, ExecutionData, ContextHandle, Damage);

	// Set incoming damage attribute.
	const FGameplayModifierEvaluatedData EvaluatedData = {UP16AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage};
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

void UP16ExecCalcDamage::AffectResistance(const FGameplayEffectCustomExecutionParameters& ExecutionParameters, FGameplayEffectContextHandle& ContextHandle, const FGameplayTag& ResistanceTag, float& OutDamage) const
{
	// For some reason FGSGameplayTagsSingleton::Get() is not initialized when FP16DamageStatics constructor is called.
	// So, instead of creating a map there, we're using static variable instead.
	// But not with all captured attributes, but only with the ones we really need.

	// clang-format off
	const auto& ResistanceTags = FGSGameplayTagsSingleton::Get().P16Tags.Attribute.Resistance;
	static TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefinitions = {
		{ResistanceTags.ArcaneTag,    FP16DamageStatics::Get().ArcaneResistanceDef},
		{ResistanceTags.FireTag,      FP16DamageStatics::Get().FireResistanceDef},
		{ResistanceTags.LightningTag, FP16DamageStatics::Get().LightningResistanceDef},
		{ResistanceTags.PhysicalTag,  FP16DamageStatics::Get().PhysicalResistanceDef},
	};
	// clang-format on

	EARLY_RETURN_IF(!TagsToCaptureDefinitions.Contains(ResistanceTag))
	const auto  CaptureDef = TagsToCaptureDefinitions[ResistanceTag];
	const float Resistance = GetAttributeMagnitude(ExecutionParameters, CaptureDef);

	OutDamage -= OutDamage * Resistance / 100.f;
}

void UP16ExecCalcDamage::AffectBlockChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const
{
	const float TargetBlockChance = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().BlockChanceDef);
	EARLY_RETURN_IF(!HasChance(TargetBlockChance))

	OutDamage /= 2.f;

	UP16AbilitySystemLibrary::SetIsBlockedHit(ContextHandle, true);
}

void UP16ExecCalcDamage::AffectArmorAndPenetration(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FP16ExecutionData& ExecutionData, float& OutDamage) const
{
	AActor* SourceAvatar = ExecutionData.SourceAvatar;
	AActor* TargetAvatar = ExecutionData.TargetAvatar;

	const float SourceLevel = SourceAvatar && SourceAvatar->Implements<UP16CombatInterface>() ? IP16CombatInterface::Execute_GetPlayerLevel(SourceAvatar) : 1.f;
	const float TargetLevel = TargetAvatar && TargetAvatar->Implements<UP16CombatInterface>() ? IP16CombatInterface::Execute_GetPlayerLevel(TargetAvatar) : 1.f;

	const float ArmorPenetrationCoefficient = GetCoefficient(SourceAvatar, "ArmorPenetration", SourceLevel);
	const float EffectiveArmorCoefficient   = GetCoefficient(TargetAvatar, "EffectiveArmor", TargetLevel);

	const float TargetArmor            = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().ArmorDef);
	const float SourceArmorPenetration = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().ArmorPenetrationDef);
	const float EffectiveArmor         = TargetArmor * (1.f - SourceArmorPenetration * ArmorPenetrationCoefficient / 100.f);
	const float FinalCoefficient       = 1.f - EffectiveArmor * EffectiveArmorCoefficient / 100.f;

	OutDamage *= FinalCoefficient;
}

void UP16ExecCalcDamage::AffectCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FP16ExecutionData& ExecutionData, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const
{
	AActor*     TargetAvatar = ExecutionData.TargetAvatar;
	const float TargetLevel  = TargetAvatar && TargetAvatar->Implements<UP16CombatInterface>() ? IP16CombatInterface::Execute_GetPlayerLevel(TargetAvatar) : 1.f;

	const float CriticalHitChance     = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().CriticalHitChanceDef);
	const float CriticalHitResistance = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().CriticalHitResistanceDef);
	const float CriticalHitDamage     = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().CriticalHitDamageDef);

	const float CriticalHitResistanceCoefficient = GetCoefficient(TargetAvatar, "CriticalHitResistance", TargetLevel);
	const float EffectiveCriticalHitChance       = CriticalHitChance * (1.f - CriticalHitResistance * CriticalHitResistanceCoefficient / 100.f);
	EARLY_RETURN_IF(!HasChance(EffectiveCriticalHitChance))

	OutDamage *= 2.f;
	OutDamage += CriticalHitDamage;

	UP16AbilitySystemLibrary::SetIsCriticalHit(ContextHandle, true);
}

bool UP16ExecCalcDamage::HasChance(const float Chance) const
{
	return Chance >= FMath::RandRange(1.f, 100.f);
}

float UP16ExecCalcDamage::GetAttributeMagnitude(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const
{
	const FGameplayEffectSpec     Spec = ExecutionParams.GetOwningSpec();
	FAggregatorEvaluateParameters EvaluateParameters;
	EvaluateParameters.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvaluateParameters.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float Magnitude = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(InCaptureDef, EvaluateParameters, Magnitude);
	Magnitude = FMath::Clamp(Magnitude, 0.f, 100.f);

	return Magnitude;
}

float UP16ExecCalcDamage::GetCoefficient(const UObject* WorldContextObject, const FName RowName, const float Level, const float DefaultValue) const
{
	const UP16CharacterClassInfoDataAsset* ClassInfo = UP16AbilitySystemLibrary::GetCharacterClassInfo(WorldContextObject);
	EARLY_RETURN_VALUE_IF(!ClassInfo, DefaultValue);
	const FRealCurve* Curve = ClassInfo->DamageCalculationCoefficients->FindCurve(RowName, FString {});
	EARLY_RETURN_VALUE_IF(!Curve, DefaultValue);

	return Curve->Eval(Level, DefaultValue);
}
