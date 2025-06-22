// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "P17GameplayAbility.generated.h"

UENUM(BlueprintType)
enum class EP17AbilityActivationPolicy : uint8
{
	OnGiven,
	OnTriggered
};

UCLASS()
class PROJECT17_API UP17GameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	/// @par UGameplayAbility interface.
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Ability")
	EP17AbilityActivationPolicy ActivationPolicy = EP17AbilityActivationPolicy::OnTriggered;
};
