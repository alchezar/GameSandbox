// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P15DashAbility.generated.h"

class FMontageWaitSimpleDelegate;
class UAbilityTask_PlayMontageAndWait;

UCLASS()
class PROJECT15_API UP15DashAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15DashAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

private:
	void OnGameplayEventReceived(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData);
	UFUNCTION()
	void OnMontageFinishedCallback();

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UAnimMontage> DashMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Impulse = 1500.f;

private:
	float  DefaultGroundFriction = 0.f;
	float* CurrentGroundFriction = nullptr;
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait*    PlayMontageTask  = nullptr;
	TArray<FMontageWaitSimpleDelegate*> MontageDelegates = {};
};
