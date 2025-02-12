// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P15BaseAbility.h"
#include "P15GroundBlastAbility.generated.h"

class AGameplayAbilityTargetActor;
class UAbilityTask_WaitTargetData;

UCLASS()
class PROJECT15_API UP15GroundBlastAbility : public UP15BaseAbility
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15GroundBlastAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	void OnValidDataReceivedCallback(const FGameplayAbilityTargetDataHandle& TargetData);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<AGameplayAbilityTargetActor> TargetActorClass;

private:
	UPROPERTY()
	UAbilityTask_WaitTargetData* WaitTargetData = nullptr;
};
