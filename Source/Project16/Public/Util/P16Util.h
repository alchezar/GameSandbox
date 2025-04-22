// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16Type.h"
#include "P16Util.generated.h"

struct FGameplayAbilitySpec;
class UP16AbilitySystemComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnDeathSignature, AActor*, DeadActor);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnAttributeChangeSignature, float, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnPlayerStatChangeSignature, int32, NewValue);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnMessageWidgetRowSignature, FP16UIWidgetRow, Row);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16AttributeInfoSignature, const FP16AttributeInfo&, AttributeInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16AbilityInfoSignature, const FP16AbilityInfo&, AbilityInfo);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16OnWaitForEquipSelectionSignature, const FGameplayTag&, TypeTag);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FP16OnActivatePassiveEffectSignature, const FGameplayTag&, AbilityTag, const bool, bActivate);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FP16OnSpellGlobeSelectedSignature, bool, bAllowSpend, bool, bAllowEquip, FString, CurrentDescription, FString, NextDescription);

DECLARE_MULTICAST_DELEGATE(FP16OnAbilitiesGivenSignature);

DECLARE_MULTICAST_DELEGATE_OneParam(FP16OnEffectAppliedSignature, const FGameplayTagContainer& /*AssetTags*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FP16OnPlayerStatChangedSignature, const int32 /*StatValue*/);

DECLARE_MULTICAST_DELEGATE_OneParam(FP16OnAbilitySystemRegisteredSignature, UAbilitySystemComponent*);

DECLARE_MULTICAST_DELEGATE_OneParam(FP16OnDeactivatePassiveAbilitySignature, const FGameplayTag& /*AbilityTag*/);

DECLARE_MULTICAST_DELEGATE_ThreeParams(FP16OnAbilityStatusChangedSignature, const FGameplayTag& /*AbilityTag*/, const FGameplayTag& /*StatusTag*/, const int32 /*Level*/);

DECLARE_MULTICAST_DELEGATE_FourParams(FP16OnAbilityEquippedSignature, const FGameplayTag& AbilityTag, const FGameplayTag& SlotInputTag, const FGameplayTag& PreviousSlotInputTag, const FGameplayTag& StatusTag)

DECLARE_DELEGATE_OneParam(FP16ForEachAbilitySignature, const FGameplayAbilitySpec&);

UCLASS()
class UP16Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
};
