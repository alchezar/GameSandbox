// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "Util/P16Util.h"
#include "P16AbilitySystemComponent.generated.h"

class UP16SaveGame;
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
	bool GetIsStartupAbilitiesGiven() const { return bStartupAbilitiesGiven; }

	_NODISCARD
	static FGameplayTag GetAbilityTagFromSpec(const FGameplayAbilitySpec& InAbilitySpec);
	_NODISCARD
	static FGameplayTag GetInputTagFromSpec(const FGameplayAbilitySpec& InAbilitySpec);
	_NODISCARD
	static FGameplayTag GetStatusFromSpec(const FGameplayAbilitySpec& InAbilitySpec);
	_NODISCARD
	FGameplayTag GetStatusFromAbilityTag(const FGameplayTag& InTag);
	_NODISCARD
	FGameplayTag GetInputTagFromAbilityTag(const FGameplayTag& InTag);
	_NODISCARD
	FP16AbilityDescription GetDescription(const FGameplayTag& AbilityTag);

	void OnAbilityActorInfoSet();
	void AddCharacterAbilitiesFromSaveData(UP16SaveGame* SaveGame);
	void AddCharacterAbilities(const TArray<TSubclassOf<UP16GameplayAbility>>& StartupAbilities);
	void AddCharacterPassiveAbilities(const TArray<TSubclassOf<UGameplayAbility>>& StartupPassiveAbilities);
	void AbilityInputTagPressed(const FGameplayTag& InputTag);
	void AbilityInputTagHeld(const FGameplayTag& InputTag);
	void AbilityInputTagReleased(const FGameplayTag& InputTag);
	void ForEachAbility(const FP16ForEachAbilitySignature& InDelegate);
	void UpdateAttribute(const FGameplayTag& AttributeTag);
	void UpdateAbilityStatuses(const int32 Level);
	void UpdateStunStatus(const bool bStun);

	UFUNCTION(Server, Reliable)
	void Server_SpendSpellPoint(const FGameplayTag& AbilityTag);
	UFUNCTION(Server, Reliable)
	void Server_EquipAbility(const FGameplayTag& InAbilityTag, const FGameplayTag& SlotInputTag);
	UFUNCTION(Client, Reliable)
	void Client_EquipAbility(const FGameplayTag& AbilityTag, const FGameplayTag& SlotInputTag, const FGameplayTag& PreviousSlotInputTag, const FGameplayTag& StatusTag);
	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_ActivatePassiveEffect(const FGameplayTag& AbilityTag, const bool bActivate);

protected:
	UFUNCTION(Client, Reliable)
	void Client_OnEffectAppliedCallback(UAbilitySystemComponent* AbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle);
	UFUNCTION(Client, Reliable)
	void Client_OnUpdateAbilityStatus(const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel);
	UFUNCTION(Server, Reliable)
	void Server_UpdateAttribute(const FGameplayTag& AttributeTag);

	static void ClearSlot(FGameplayAbilitySpec* InSpec);
	void        ClearAbilitiesOfSlot(const FGameplayTag& InSlotInputTag);

private:
	bool                  GetSlotIsEmpty(const FGameplayTag& InSlotInputTag);
	static bool           GetAbilityHasSlot(const FGameplayAbilitySpec& Spec, const FGameplayTag& Slot);
	static bool           GetAbilityHasAnySlot(const FGameplayAbilitySpec& Spec);
	FGameplayAbilitySpec* GetSpecWithSlot(const FGameplayTag& InSlotInputTag);
	bool                  GetIsPassiveAbility(const FGameplayAbilitySpec& Spec) const;
	static void           AssignSlotToAbility(FGameplayAbilitySpec& Spec, const FGameplayTag& SlotInputTag);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	FP16OnEffectAppliedSignature            OnEffectApplied;
	FP16OnAbilitiesGivenSignature           OnAbilitiesGiven;
	FP16OnAbilityStatusChangedSignature     OnAbilityStatusChanged;
	FP16OnAbilityEquippedSignature          OnAbilityEquipped;
	FP16OnDeactivatePassiveAbilitySignature OnDeactivatePassiveAbility;
	FP16OnActivatePassiveEffectSignature    OnActivatePassiveEffect;

private:
	bool bStartupAbilitiesGiven = false;
};
