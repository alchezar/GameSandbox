// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P15BaseAbility.h"
#include "P15LaserAbility.generated.h"

class FMontageWaitSimpleDelegate;
class UAbilityTask_PlayMontageAndWait;

UCLASS()
class PROJECT15_API UP15LaserAbility : public UP15BaseAbility
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15LaserAbility();

	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnDamageTagReceivedCallback(FGameplayTag Tag, const FGameplayEventData* Payload);

private:
	void            SpawnLaserActor();
	TArray<AActor*> GetTargetActors(const FGameplayAbilityTargetDataHandle& TargetData) const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Montage")
	TObjectPtr<UAnimMontage> LaserMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Montage")
	FName StartSection = "Start";

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Laser")
	TSubclassOf<AActor> LaserActorClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Laser")
	FName AttachSocketName = "P15_Hand";
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Laser")
	float SpawnDelay = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	TSubclassOf<UGameplayEffect> DamageEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	FGameplayTag DamageTag = FGameplayTag::RequestGameplayTag("p15.skill.laser.deal_damage");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	float PushStrength = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Damage")
	FGameplayTag ManaDrainTag = FGameplayTag::RequestGameplayTag("p15.skill.laser.mana_drain");

private:
	FTimerHandle SpawnTimer;
	UPROPERTY()
	UAbilityTask_PlayMontageAndWait* LaserMontageTask = nullptr;
	UPROPERTY()
	AActor* LaserActor = nullptr;
};
