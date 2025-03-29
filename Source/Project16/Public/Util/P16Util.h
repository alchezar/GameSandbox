// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16Type.h"
#include "P16Util.generated.h"

struct FGameplayAbilitySpec;
class UP16AbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnAttributeChangeSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnMessageWidgetRowSignature, FP16UIWidgetRow, Row);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16AttributeInfoSignature, const FP16AttributeInfo&, AttributeInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16AbilityInfoSignature, const FP16AbilityInfo&, AbilityInfo);

DECLARE_MULTICAST_DELEGATE_OneParam(FP16OnEffectAppliedSignature, const FGameplayTagContainer& /*AssetTags*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FP16OnAbilitiesGivenSignature, UP16AbilitySystemComponent* /*AbilitySystem*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FP16OnPlayerStatChangedSignature, const int32 /*StatValue*/);

DECLARE_DELEGATE_OneParam(FP16ForEachAbilitySignature, const FGameplayAbilitySpec&);

UCLASS()
class UP16Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
