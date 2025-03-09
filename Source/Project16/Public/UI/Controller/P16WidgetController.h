// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Util/P16Type.h"
#include "P16WidgetController.generated.h"

class UAttributeSet;
class UAbilitySystemComponent;

UCLASS()
class PROJECT16_API UP16WidgetController : public UObject
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	void InitWidgetController(const FP16WidgetControllerParams& InParams);
	UFUNCTION(BlueprintCallable)
	void SetWidgetControllerParams(const FP16WidgetControllerParams& InParams);

	virtual void BindCallbacksToDependencies();
	UFUNCTION(BlueprintCallable)
	virtual void BroadcastInitialValues();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++ | Widget Controller")
	FP16WidgetControllerParams Params = {};
};
