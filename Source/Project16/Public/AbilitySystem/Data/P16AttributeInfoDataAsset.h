// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Engine/DataAsset.h"
#include "Util/P16Type.h"
#include "P16AttributeInfoDataAsset.generated.h"

UCLASS()
class PROJECT16_API UP16AttributeInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	FP16AttributeInfo FindAttributeInfo(const FGameplayTag& InTag, const UAttributeSet* InAttributeSet, const bool bLogNotFound = false) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FP16AttributeInfo> AttributeInfos;
};
