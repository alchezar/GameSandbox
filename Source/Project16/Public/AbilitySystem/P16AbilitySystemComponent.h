// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Util/P16Util.h"
#include "P16AbilitySystemComponent.generated.h"

class UP16GameplayAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT16_API UP16AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP16AbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/* ------------------------------- This -------------------------------- */
public:
	void OnAbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UP16GameplayAbility>>& StartupAbilities);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);

protected:
	UFUNCTION(Client, Reliable)
	void Client_OnEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	/* ------------------------------ Fields ------------------------------- */
public:
	FP16OnEffectAppliedSignature OnEffectApplied;
};
