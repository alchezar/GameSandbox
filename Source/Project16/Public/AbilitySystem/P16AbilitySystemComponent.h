// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Util/P16Util.h"
#include "P16AbilitySystemComponent.generated.h"

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

protected:
	void OnEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);

	/* ------------------------------ Fields ------------------------------- */
public:
	FP16OnEffectAppliedSignature OnEffectApplied;
};
