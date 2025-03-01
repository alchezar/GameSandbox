// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16WidgetController.h"
#include "Util/P16Util.h"
#include "P16OverlayWidgetController.generated.h"

class UP16AttributeSet;
struct FOnAttributeChangeData;

UCLASS(BlueprintType, Blueprintable)
class PROJECT16_API UP16OverlayWidgetController : public UP16WidgetController
{
	GENERATED_BODY()

	/* ------------------------------- Super ------------------------------- */
public:
	virtual void BroadcastInitialValues() override;
	virtual void BindCallbacksToDependencies() override;

	/* ------------------------------- This -------------------------------- */
protected:
	void OnHealthChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnManaChangedCallback(const FOnAttributeChangeData& Data) const;
	void OnMaxManaChangedCallback(const FOnAttributeChangeData& Data) const;

	void OnEffectAppliedCallback(const FGameplayTagContainer& AssetTags);

	/* ------------------------------ Fields ------------------------------- */
public:
	UPROPERTY(BlueprintAssignable, Category = "C++ | Attributes")
	FP16OnHealthChangeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Attributes")
	FP16OnMaxHealthChangeSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Attributes")
	FP16OnManaChangeSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Attributes")
	FP16OnMaxManaChangeSignature OnMaxManaChanged;
};
