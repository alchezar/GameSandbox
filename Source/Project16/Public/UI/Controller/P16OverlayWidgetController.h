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
	template <typename T>
	T* GetDataTableRowByTag(UDataTable* InDataTable, const FGameplayTag& InTag);

	/* ------------------------------ Fields ------------------------------- */
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Widget Data")
	FGameplayTag MessageTag = FGameplayTag::RequestGameplayTag("P16.Message");
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Widget Data")
	TObjectPtr<UDataTable> MessageWidgetDataTable = {};
};
