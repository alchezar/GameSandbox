// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P15TargetAbility.h"
#include "P15GroundBlastAbility.generated.h"

class UNiagaraSystem;

UCLASS()
class PROJECT15_API UP15GroundBlastAbility : public UP15TargetAbility
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15GroundBlastAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

	/* ------------------------------- Super ------------------------------- */
protected:
	virtual void OnValidDataReceivedCallback(const FGameplayAbilityTargetDataHandle& TargetData) override;
	virtual void OnValidDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetData) override;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Montage")
	TObjectPtr<UAnimMontage> BlastMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Montage")
	FName StartSection = "Start";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Montage")
	FName EndSection = "End";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	TSubclassOf<UGameplayEffect> DamageEffectClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	float PushStrength = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Effect")
	TObjectPtr<UNiagaraSystem> Niagara = nullptr;
};
