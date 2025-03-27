// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemGlobals.h"
#include "P16AbilitySystemGlobals.generated.h"

UCLASS()
class PROJECT16_API UP16AbilitySystemGlobals : public UAbilitySystemGlobals
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------

public:
	/// @brief Allocates our custom gameplay effect context.
	_NODISCARD virtual FGameplayEffectContext* AllocGameplayEffectContext() const override;
};
