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

private:
	void UpdateButtons();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16OnPlayerStatChangeSignature SpellPointsDelegate;
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16OnSpellGlobeSelectedSignature OnSpellGlobeSelected;

private:
	FP16SelectedAbility SelectedAbility = {};
};
