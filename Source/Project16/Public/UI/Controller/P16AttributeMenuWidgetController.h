// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P16WidgetController.h"
#include "Util/P16Util.h"
#include "P16AttributeMenuWidgetController.generated.h"

class UP16AttributeInfoDataAsset;

UCLASS(BlueprintType, Blueprintable)
class PROJECT16_API UP16AttributeMenuWidgetController : public UP16WidgetController
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
	void UpdateAttribute(const FGameplayTag& AttributeTag);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16AttributeInfoSignature AttributeInfoDelegate;
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16OnPlayerStatChangeSignature AttributePointsDelegate;
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16OnPlayerStatChangeSignature SpellPointsDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UP16AttributeInfoDataAsset> AttributeInfoData;
};
