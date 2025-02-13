// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P15BaseAbility.h"
#include "P15TargetAbility.generated.h"

class AP15GroundSelectTarget;
class AGameplayAbilityTargetActor;
class UAbilityTask_WaitTargetData;

UCLASS()
class PROJECT15_API UP15TargetAbility : public UP15BaseAbility
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15TargetAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	virtual void OnValidDataReceivedCallback(const FGameplayAbilityTargetDataHandle& TargetData);
	UFUNCTION()
	virtual void OnValidDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetData);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Target")
	TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass;

private:
	UPROPERTY()
	AP15GroundSelectTarget* TargetActor = nullptr;
	UPROPERTY()
	UAbilityTask_WaitTargetData* WaitTargetData = nullptr;
};
