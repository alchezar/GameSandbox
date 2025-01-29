// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P15MeleeAbility.generated.h"

class AP15Character;
class UAbilityTask_WaitGameplayEvent;

UCLASS()
class PROJECT15_API UP15MeleeAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UP15MeleeAbility();

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnAttackEnds(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FGameplayTag DealDamageTag = FGameplayTag::RequestGameplayTag("p15.melee.deal_damage");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UAnimMontage* InstigatorMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UAnimMontage* TargetMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName StartAttackMontageSectionName = "Start";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	FName EndAttackMontageSectionName = "End";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;

private:
	UPROPERTY()
	TObjectPtr<AP15Character> Char = nullptr;
	UPROPERTY()
	TObjectPtr<AP15Character> Enemy = nullptr;
};
