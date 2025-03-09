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
	/// @name This
	/// ------------------------------------------------------------------------
public:
	virtual void BindCallbacksToDependencies() override;
	virtual void BroadcastInitialValues() override;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(BlueprintAssignable, Category = "C++")
	FP16AttributeInfoSignature AttributeInfoDelegate;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TObjectPtr<UP16AttributeInfoDataAsset> AttributeInfoData;
};
