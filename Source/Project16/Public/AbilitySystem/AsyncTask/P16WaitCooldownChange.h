// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "P16WaitCooldownChange.generated.h"

class UP16AbilitySystemComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP16CooldownChangeSignature, float, TimeRemaining);

UCLASS(BlueprintType, meta = (ExposedAsyncProxy = "AsyncTask"))
class PROJECT16_API UP16WaitCooldownChange : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static UP16WaitCooldownChange* WaitCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayTag& InCooldownTag);

	UFUNCTION(BlueprintCallable)
	void EndTask();

	void OnCooldownTagChangedCallback(FGameplayTag InCooldownTag, const int NewCount);
	void OnActiveEffectAddedCallback(UAbilitySystemComponent* TargetAbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable)
	FP16CooldownChangeSignature CooldownStart;
	UPROPERTY(BlueprintAssignable)
	FP16CooldownChangeSignature CooldownEnd;

protected:
	FGameplayTag CooldownTag = {};
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent = nullptr;
};
