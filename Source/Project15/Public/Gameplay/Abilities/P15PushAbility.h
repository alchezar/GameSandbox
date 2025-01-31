// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P15PushAbility.generated.h"

UCLASS()
class PROJECT15_API UP15PushAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15PushAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

	/* ------------------------------- This -------------------------------- */
private:
	void DrawDebug(const FHitResult& InResult) const;
	///
	/// @brief Find the name of the animation section.
	/// @param Target Actor that was hit
	/// @param Culprit Actor that caused the hit
	/// @return Name of the animation section
	///
	FName FindDirectionName(const AActor* Target, const AActor* Culprit) const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UAnimMontage* PushMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	UAnimMontage* ReactMontage = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UGameplayEffect> DamageGameplayEffectClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	float PushDistance = 400.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	bool bDebug = true;
};
