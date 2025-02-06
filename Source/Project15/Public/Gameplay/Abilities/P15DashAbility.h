// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P15BaseAbility.h"
#include "P15DashAbility.generated.h"

class UNiagaraSystem;
class FMontageWaitSimpleDelegate;
class UAbilityTask_PlayMontageAndWait;

UCLASS()
class PROJECT15_API UP15DashAbility : public UP15BaseAbility
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
	TObjectPtr<UNiagaraSystem> StunVfx = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float Impulse = 1500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float PushStrength = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float StunDuration = 5.f;

private:
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait*    PlayMontageTask  = nullptr;
	TArray<FMontageWaitSimpleDelegate*> MontageDelegates = {};
	UPROPERTY()
	TSet<AActor*> OverlapExclude = {};
};
