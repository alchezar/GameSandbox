// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P16LoadScreenWidget.generated.h"

UCLASS()
class PROJECT16_API UP16LoadScreenWidget : public UUserWidget
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void BP_InitSlots();
};
