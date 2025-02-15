// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Gameplay/Abilities/P15BaseAbility.h"
#include "P15FireBlastAbility.generated.h"

class UAbilityTask_WaitTargetData;
class AP15AroundTarget;

UCLASS()
class PROJECT15_API UP15FireBlastAbility : public UP15BaseAbility
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15FireBlastAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/* ------------------------------- This -------------------------------- */
private:
	void ListenForAnimNotifies(UAnimMontage* InMontage);
	void OnFireBlastPullCallback(USkeletalMeshComponent* MeshComp);
	void OnFireBlastPushCallback(USkeletalMeshComponent* MeshComp);
	UFUNCTION()
	void OnDataReceivedCallback(const FGameplayAbilityTargetDataHandle& TargetData);
	UFUNCTION()
	void OnDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetData);

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Montage")
	TObjectPtr<UAnimMontage> BlastMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Montage")
	FName StartSectionName = "Start";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Target")
	TSubclassOf<AP15AroundTarget> TargetActorClass = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | PushPull")
	float PullStrength = 500.f;

private:
	UPROPERTY()
	AP15AroundTarget* TargetActor = nullptr;
	UPROPERTY()
	UAbilityTask_WaitTargetData* WaitTargetData = nullptr;
};
