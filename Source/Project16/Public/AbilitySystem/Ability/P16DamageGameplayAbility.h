// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16GameplayAbility.h"
#include "P16DamageGameplayAbility.generated.h"

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
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Class")
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Damage")
	TMap<FGameplayTag, FScalableFloat> DamageTypes;
};
