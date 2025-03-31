// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16WidgetController.h"
#include "Util/P16Util.h"
#include "P16OverlayWidgetController.generated.h"

class UP16AbilitySystemComponent;
class UP16AbilityInfoDataAsset;
class UP16AttributeSet;
struct FOnAttributeChangeData;

UCLASS(BlueprintType, Blueprintable)
class PROJECT16_API UP16OverlayWidgetController : public UP16WidgetController
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
protected:
	template <typename T>
	T* GetDataTableRowByTag(UDataTable* InDataTable, const FGameplayTag& InTag);

	void OnInitStartupAbilities(UP16AbilitySystemComponent* AbilitySystem);
	void OnEffectAppliedCallback(const FGameplayTagContainer& AssetTags);
	void OnXPChangedCallback(const int32 NewXP) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable, Category = "C++ | Attributes")
	FP16OnAttributeChangeSignature OnHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Attributes")
	FP16OnAttributeChangeSignature OnMaxHealthChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Attributes")
	FP16OnAttributeChangeSignature OnManaChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Attributes")
	FP16OnAttributeChangeSignature OnMaxManaChanged;

	UPROPERTY(BlueprintAssignable, Category = "C++ | Messages")
	FP16OnMessageWidgetRowSignature OnMessageWidgetRow;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Messages")
	FP16AbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(BlueprintAssignable, Category = "C++ | Stat")
	FP16OnAttributeChangeSignature OnXPPercentChanged;
	UPROPERTY(BlueprintAssignable, Category = "C++ | Stat")
	FP16OnPlayerStatChangeSignature OnLevelChanged;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Widget Data")
	FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("P16.Message");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable = {};
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Widget Data")
	TObjectPtr<UP16AbilityInfoDataAsset> AbilityInfo = {};
};
