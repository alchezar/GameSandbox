// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16DamageGameplayAbility.h"
#include "P16MeleeAttackAbility.generated.h"

UCLASS()
class PROJECT16_API UP16MeleeAttackAbility : public UP16DamageGameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "C++")
	TObjectPtr<UAnimMontage> AttachMontage = nullptr;
};
