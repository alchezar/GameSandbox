// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Util/P16Util.h"
#include "P16AbilitySystemComponent.generated.h"

class UP16GameplayAbility;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECT16_API UP16AbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	UP16AbilitySystemComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(const float DeltaTime, const ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void OnRep_ActivateAbilities() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	FGameplayAbilitySpec* GetSpecFromAbilityTag(const FGameplayTag& InTag);

	_NODISCARD
	bool GetIsStartupAbilitiesGiven() const { return bStartupAbilitiesGiven; };
	_NODISCARD
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& InAbilitySpec);
	_NODISCARD
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& InAbilitySpec);
	_NODISCARD
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& InAbilitySpec);

	void OnAbilityActorInfoSet();
	void AddCharacterAbilities(const TArray<TSubclassOf<UP16GameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FP16ForEachAbilitySignature& InDelegate);
	void UpdateAttribute(const FGameplayTag& AttributeTag);
	void UpdateAbilityStatuses(const int32 Level);

protected:
	UFUNCTION(Client, Reliable)
	void Client_OnEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	UFUNCTION(Client, Reliable)
	void Client_OnUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag);
	UFUNCTION(Server, Reliable)
	void Server_UpdateAttribute(const FGameplayTag& AttributeTag);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	FP16OnEffectAppliedSignature        OnEffectApplied;
	FP16OnAbilitiesGivenSignature       OnAbilitiesGiven;
	FP16OnAbilityStatusChangedSignature OnAbilityStatusChanged;

private:
	bool bStartupAbilitiesGiven = false;
};
