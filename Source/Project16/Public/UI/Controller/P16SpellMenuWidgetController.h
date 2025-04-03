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
protected:
};
