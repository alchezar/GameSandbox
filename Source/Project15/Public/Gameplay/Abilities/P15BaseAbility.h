// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Utils/P15Types.h"
#include "P15BaseAbility.generated.h"

class AP15Character;

UCLASS()
class PROJECT15_API UP15BaseAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
public:
	UP15BaseAbility();

protected:
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled) override;

	/* ------------------------------- This -------------------------------- */
public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	FP15AbilityInfo GetAbilityInfo() const;

private:
	bool                GetIsInstant(const UGameplayEffect* InEffect) const;
	EP15AbilityCostType GetCostTypeByName(const FString& InName) const;

	/* ------------------------------ Fields ------------------------------- */
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UMaterialInterface> UIMaterial = nullptr;

	UPROPERTY()
	TObjectPtr<AP15Character> Char = nullptr;
};
