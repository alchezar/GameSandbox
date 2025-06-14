// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "P16ExecCalcDamage.generated.h"

struct FP16GameplayEffectContext;
class IP16CombatInterface;

/// ----------------------------------------------------------------------------
/// @class   FP16ExecutionData
/// @brief   Represents a container for execution data.
/// @details Combines all necessary data from the execution parameters.
/// ----------------------------------------------------------------------------
struct FP16ExecutionData
{
	UAbilitySystemComponent* SourceAbilitySystemComponent = nullptr;
	UAbilitySystemComponent* TargetAbilitySystemComponent = nullptr;

	AActor* SourceAvatar = nullptr;
	AActor* TargetAvatar = nullptr;

	explicit FP16ExecutionData(const FGameplayEffectCustomExecutionParameters& ExecutionParams);
};

/// ----------------------------------------------------------------------------
/// @class   UP16ExecCalcDamage
/// @brief   Represents the calculation of damage.
/// @details Extends the base class with an ability to calculate damage
/// and apply all necessary damage modifiers.
/// ----------------------------------------------------------------------------
UCLASS()
class PROJECT16_API UP16ExecCalcDamage : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP16ExecCalcDamage();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
private:
	FGameplayEffectAttributeCaptureDefinition& GetCaptureDefinitionBy(const FGameplayTag& InTag) const;

	void AffectDamage(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const;
	/// @brief Apply resistance to each damage type.
	void AffectResistance(const FGameplayEffectCustomExecutionParameters& ExecutionParameters, FGameplayEffectContextHandle& ContextHandle, const FGameplayTag& ResistanceTag, float& OutDamage) const;
	/// @brief If this is a radial damage - scale the damage down based on the distance from origin.
	void AffectRadialDistance(const FGameplayEffectCustomExecutionParameters& ExecutionParameters, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const;
	/// @brief Capture the block chance on the target and determine if there was a successful block. If so, half the damage.
	void AffectBlockChance(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const;
	/// @brief Armor penetration ignores a percentage of the target's armor.
	void AffectArmorAndPenetration(const FGameplayEffectCustomExecutionParameters& ExecutionParams, float& OutDamage) const;
	/// @brief Critical hits have a chance to double the damage.
	void AffectCriticalHit(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectContextHandle& ContextHandle, float& OutDamage) const;
	/// @brief Apply debuffs to the target.
	void AffectDebuffs(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectSpec& Spec, FGameplayEffectContextHandle& ContextHandle) const;

	bool  HasChance(const float Chance) const;
	float GetAttributeMagnitude(const FGameplayEffectCustomExecutionParameters& ExecutionParams, const FGameplayEffectAttributeCaptureDefinition& InCaptureDef) const;
	float GetCoefficient(const UObject* WorldContextObject, const FName RowName, const float Level, const float DefaultValue = 1.f) const;
};
