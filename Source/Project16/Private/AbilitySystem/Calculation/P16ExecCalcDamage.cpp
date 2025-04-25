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
	const FGameplayEffectSpec    Spec          = ExecutionParams.GetOwningSpec();
	FGameplayEffectContextHandle ContextHandle = Spec.GetContext();

	float Damage = 0.f;

	AffectDamage(ExecutionParams, Spec, ContextHandle, Damage);
	AffectBlockChance(ExecutionParams, ContextHandle, Damage);
	AffectArmorAndPenetration(ExecutionParams, Damage);
	AffectCriticalHit(ExecutionParams, ContextHandle, Damage);
	AffectDebuffs(ExecutionParams, Spec, ContextHandle);

	// Set incoming damage attribute.
	const FGameplayModifierEvaluatedData EvaluatedData = {UP16AttributeSet::GetIncomingDamageAttribute(), EGameplayModOp::Override, Damage};
	OutExecutionOutput.AddOutputModifier(EvaluatedData);
}

FGameplayEffectAttributeCaptureDefinition& UP16ExecCalcDamage::GetCaptureDefinitionBy(const FGameplayTag& InTag) const
{
	//! For some reason FGSGameplayTagsSingleton::Get() is not initialized when FP16DamageStatics constructor is called.
	//! So, instead of creating a map there, we're using static variable instead.

	const auto& SecondaryTags  = FGSGameplayTagsSingleton::Get().P16Tags.Attribute.Secondary;
	const auto& ResistanceTags = FGSGameplayTagsSingleton::Get().P16Tags.Attribute.Resistance;
	// clang-format off
	static TMap<FGameplayTag, FGameplayEffectAttributeCaptureDefinition> TagsToCaptureDefinitions = {
		{SecondaryTags.ArmorTag,                 FP16DamageStatics::Get().ArmorDef},
		{SecondaryTags.ArmorPenetrationTag,      FP16DamageStatics::Get().ArmorPenetrationDef},
		{SecondaryTags.BlockChanceTag,           FP16DamageStatics::Get().BlockChanceDef},
		{SecondaryTags.CriticalHitChanceTag,     FP16DamageStatics::Get().CriticalHitChanceDef},
		{SecondaryTags.CriticalHitDamageTag,     FP16DamageStatics::Get().CriticalHitDamageDef},
		{SecondaryTags.CriticalHitResistanceTag, FP16DamageStatics::Get().CriticalHitResistanceDef},
		{ResistanceTags.ArcaneTag,    FP16DamageStatics::Get().ArcaneResistanceDef},
		{ResistanceTags.FireTag,      FP16DamageStatics::Get().FireResistanceDef},
		{ResistanceTags.LightningTag, FP16DamageStatics::Get().LightningResistanceDef},
		{ResistanceTags.PhysicalTag,  FP16DamageStatics::Get().PhysicalResistanceDef},
	};
	// clang-format on

	return TagsToCaptureDefinitions[InTag];
}

void UP16ExecCalcDamage::AffectDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const
{
	// Search through all damage types.
	for (const auto& [DamageTag, ResistanceTag] : FGSGameplayTagsSingleton::Get().P16Tags.Maps.DamageTypesToResistances)
	{
		float DamageTypeValue = Spec.GetSetByCallerMagnitude(DamageTag, false);
		AffectResistance(ExecutionParams, ContextHandle, ResistanceTag, DamageTypeValue);
		AffectRadialDistance(ExecutionParams, ContextHandle, DamageTypeValue);
		OutDamage += DamageTypeValue;
	}
}

void UP16ExecCalcDamage::AffectResistance(const FGameplayEffectCustomExecutionParameters& ExecutionParameters, FGameplayEffectContextHandle& ContextHandle, const FGameplayTag& ResistanceTag, float& OutDamage) const
{
	const auto& CaptureDef = GetCaptureDefinitionBy(ResistanceTag);
	const float Resistance = GetAttributeMagnitude(ExecutionParameters, CaptureDef);

	OutDamage -= OutDamage * Resistance / 100.f;
}

void UP16ExecCalcDamage::AffectRadialDistance(const FGameplayEffectCustomExecutionParameters& ExecutionParameters, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const
{
	const FP16ExecutionData      ExecutionData {ExecutionParameters};
	const FP16RadialDamageParams RadialDamageParams = UP16AbilitySystemLibrary::GetRadialDamageParams(ContextHandle);
	EARLY_RETURN_IF(!RadialDamageParams.bRadial)

#if 0
	// Approach with using `UGameplayStatics::ApplyRadialDamageWithFalloff` method.
	if (const TScriptInterface<IP16CombatInterface> CombatInterface = ExecutionData.TargetAvatar)
	{
		CombatInterface->GetOnRadialDamageDelegate().AddWeakLambda(this, [this, &OutDamage](const float Damage) -> void
		{
			OutDamage = Damage;
		});
		UGameplayStatics::ApplyRadialDamageWithFalloff(
			ExecutionData.TargetAvatar,
			OutDamage,
			0.f,
			RadialDamageParams.Origin,
			RadialDamageParams.InnerRadius,
			RadialDamageParams.OuterRadius,
			1.f,
			UDamageType::StaticClass(),
			{},
			ExecutionData.SourceAvatar);
	}
#endif

	// My simple approach.
	const FVector TargetLocation = ExecutionData.TargetAvatar->GetActorLocation();
	const float   Distance       = (TargetLocation - RadialDamageParams.Origin).Size();

	if (Distance <= RadialDamageParams.InnerRadius)
	{
		return;
	}
	if (Distance >= RadialDamageParams.OuterRadius)
	{
		OutDamage = 0.f;
		return;
	}
	const float Range = RadialDamageParams.OuterRadius - RadialDamageParams.InnerRadius;
	const float Delta = Distance - RadialDamageParams.InnerRadius;
	const float Alpha = 1.f - Delta / FMath::Max(1.f, Range);
	OutDamage *= Alpha;
}

void UP16ExecCalcDamage::AffectBlockChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const
{
	const float TargetBlockChance = GetAttributeMagnitude(ExecutionParams, FP16DamageStatics::Get().BlockChanceDef);
	EARLY_RETURN_IF(!HasChance(TargetBlockChance))

	OutDamage /= 2.f;

	UP16AbilitySystemLibrary::SetIsBlockedHit(ContextHandle, true);
}

void UP16ExecCalcDamage::AffectArmorAndPenetration(const FGameplayEffectCustomExecutionParameters& ExecutionParams, float& OutDamage) const
{
	const FP16ExecutionData ExecutionData {ExecutionParams};

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

void UP16ExecCalcDamage::AffectCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const
{
	const FP16ExecutionData ExecutionData {ExecutionParams};

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

void UP16ExecCalcDamage::AffectDebuffs(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FGameplayEffectContextHandle& ContextHandle) const
{
	FP16Tags Tags = FGSGameplayTagsSingleton::Get().P16Tags;
	for (const auto& [DamageType, DebuffType] : Tags.Maps.DamageTypesToDebuffs)
	{
		const float TypeDamage = Spec.GetSetByCallerMagnitude(DamageType, false, -1.f);
		CONTINUE_IF(TypeDamage < 0.f)

		// Determine if there was a successful debuff.
		const FGameplayTag& ResistanceTag = Tags.Maps.DamageTypesToResistances[DamageType];
		const auto&         CaptureDef    = GetCaptureDefinitionBy(ResistanceTag);

		const float SourceDebuffChance     = Spec.GetSetByCallerMagnitude(Tags.Debuff.Info.ChanceTag, false, -1.f);
		const float TargetDebuffResistance = GetAttributeMagnitude(ExecutionParams, CaptureDef);
		const float EffectiveDebuffChance  = SourceDebuffChance * (100.f - TargetDebuffResistance) / 100.f;
		CONTINUE_IF(!HasChance(EffectiveDebuffChance))

		// Carrying debuff information through, from activation gameplay effect to the target's attribute set.
		FP16DebuffInfo DebuffInfo = {
			.Chance = 100.f,
			.Damage = Spec.GetSetByCallerMagnitude(Tags.Debuff.Info.DamageTag, false, -1.f),
			.Frequency = Spec.GetSetByCallerMagnitude(Tags.Debuff.Info.FrequencyTag, false, -1.f),
			.Duration = Spec.GetSetByCallerMagnitude(Tags.Debuff.Info.DurationTag, false, -1.f)
		};
		UP16AbilitySystemLibrary::SetDebuffSpec(ContextHandle, true, DebuffInfo, DamageType);
	}
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
