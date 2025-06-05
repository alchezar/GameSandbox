// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Util/P16Type.h"
#include "P16InputConfig.generated.h"

UCLASS()
class PROJECT16_API UP16InputConfig : public UDataAsset
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	const UInputAction* FindAbilityInputAction(const FGameplayTag& InTag, const bool bLogNotFound = false) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++ | Input")
	TArray<FP16InputAction> AbilityInputActions = {};
};
