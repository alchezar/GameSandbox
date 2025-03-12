// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P16GameplayAbility.generated.h"

/// ----------------------------------------------------------------------------
/// @class   UP16GameplayAbility
/// @brief   Represents a base gameplay ability class.
/// @details Extends the base class with specifying a startup input tag.
/// ----------------------------------------------------------------------------
UCLASS()
class PROJECT16_API UP16GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Input")
	FGameplayTag StartupInputTag;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Damage")
	FScalableFloat Damage;
};
