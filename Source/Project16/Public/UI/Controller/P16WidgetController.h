// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Util/P16Type.h"
#include "Util/P16Util.h"
#include "P16WidgetController.generated.h"

class UP16AttributeSet;
class UP16AbilityInfoDataAsset;
class UP16AbilitySystemComponent;

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

protected:
	void BroadcastEachAbilityInfo();

	AP16PlayerController*       GetAuraPlayerController();
	AP16PlayerState*            GetAuraPlayerState();
	UP16AbilitySystemComponent* GetAuraAbilitySystemComponent();
	UP16AttributeSet*           GetAuraAttributeSet();

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "C++ | Widget Controller")
	FP16WidgetControllerParams Params = {};

	UPROPERTY(BlueprintAssignable, Category = "C++ | Messages")
	FP16AbilityInfoSignature AbilityInfoDelegate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Widget Data")
	TObjectPtr<UP16AbilityInfoDataAsset> AbilityInfo = {};

private:
	FP16WidgetControllerSpecificParams SpecificParams = {};
};
