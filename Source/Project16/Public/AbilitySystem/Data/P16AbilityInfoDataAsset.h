// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Util/P16Type.h"
#include "P16AbilityInfoDataAsset.generated.h"

UCLASS()
class PROJECT16_API UP16AbilityInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	FP16AbilityInfo FindAbilityInfo(const FGameplayTag& InAbilityTag, const bool bLogNotFound = false) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FP16AbilityInfo> AbilityInfos = {};
};
