// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "Util/P17Types.h"
#include "P17GameplayAbility.generated.h"

class UP17AbilitySystemComponent;
class UP17CombatPawnComponent;

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
	/// @par UGameplayAbility interface ----------------------------------------
	virtual void OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec) override;
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	UP17CombatPawnComponent* GetCombatPawnComponentFromActorInfo() const;
	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	UP17AbilitySystemComponent* GetProjectAbilitySystemComponentFromActorInfo() const;

protected:
	FActiveGameplayEffectHandle NativeApplyEffectSpecHandleToTarget(AActor* InTarget, const FGameplayEffectSpecHandle& InSpecHandle) const;
	UFUNCTION(BlueprintCallable, Category = "C++ | Ability", meta = (DisplayName = "ApplyEffectSpecHandleToTarget", ExpandEnumAsExecs = "OutExecs"))
	FActiveGameplayEffectHandle BP_ApplyEffectSpecHandleToTarget(AActor* InTarget, const FGameplayEffectSpecHandle& InSpecHandle, EP17SuccessTypePin& OutExecs);

	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	static FName GetMontageSectionAt(const UAnimMontage* InMontage, const int32 InIndex);
	UFUNCTION(BlueprintPure, Category = "C++ | Ability")
	static FName GetMontageSectionRandom(const UAnimMontage* InMontage);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Ability")
	EP17AbilityActivationPolicy ActivationPolicy = EP17AbilityActivationPolicy::OnTriggered;
};
