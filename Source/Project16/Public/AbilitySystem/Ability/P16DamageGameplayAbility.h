// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16GameplayAbility.h"
#include "Util/P16Type.h"
#include "P16DamageGameplayAbility.generated.h"

struct FP16TaggedMontage;

/// ----------------------------------------------------------------------------
/// @class   UP16DamageGameplayAbility
/// @brief   Represents a damage gameplay ability class.
/// @details Extends the base class with specifying a tag for the damage type.
/// ----------------------------------------------------------------------------
UCLASS()
class PROJECT16_API UP16DamageGameplayAbility : public UP16GameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	void CauseDamage(AActor* TargetActor);

	UFUNCTION(BlueprintPure)
	static FP16TaggedMontage GetRandomTaggedMontageFrom(const TArray<FP16TaggedMontage>& Montages);

	UFUNCTION(BlueprintPure)
	FP16DamageEffectParams MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor = nullptr) const;

	static void UpdateDamageEffectParams(AActor* TargetActor, FP16DamageEffectParams& DamageEffectParams);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Class")
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Damage")
	FP16DamageInfo DamageInfo;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Damage")
	FP16DebuffInfo DebuffInfo = {};

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Trick")
	FP16TrickImpulse DeathImpulse = {.Magnitude = 5000.f};
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Trick")
	FP16TrickImpulse KnockbackForce = {.Magnitude = 500.f, .Chance = 0.f};
};
