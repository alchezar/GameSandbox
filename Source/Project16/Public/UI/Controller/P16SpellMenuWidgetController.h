// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16WidgetController.h"
#include "P16SpellMenuWidgetController.generated.h"

UCLASS(BlueprintType, Blueprintable)
class PROJECT16_API UP16SpellMenuWidgetController : public UP16WidgetController
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Super
	/// ------------------------------------------------------------------------
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintCallable)
	void SpellGlobeSelected(const FGameplayTag& AbilityTag);
	UFUNCTION(BlueprintCallable)
	void SpellGlobeDeselected();
	UFUNCTION(BlueprintCallable)
	void SpendPointAttempt();
	UFUNCTION(BlueprintCallable)
	void EquipButtonPressed();
	UFUNCTION(BlueprintCallable)
	void SpellRowGlobePressed(const FGameplayTag& SlotInputTag, const FGameplayTag& TypeTag);

protected:
	void OnAbilityEquippedCallback(const FGameplayTag& AbilityTag, const FGameplayTag& SlotInputTag, const FGameplayTag& PreviousSlotInputTag, const FGameplayTag& StatusTag);

private:
	void UpdateButtons();
	void StopWaitingForEquip();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16OnPlayerStatChangeSignature SpellPointsDelegate;
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16OnSpellGlobeSelectedSignature OnSpellGlobeSelected;
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16OnWaitForEquipSelectionSignature OnWaitForEquipSelection;
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16OnWaitForEquipSelectionSignature StopWaitForEquipSelection;

private:
	FP16SelectedAbility SelectedAbility           = {};
	bool                bWaitingForEquipSelection = false;
	FGameplayTag        SelectedSlotTag           = {};
};
