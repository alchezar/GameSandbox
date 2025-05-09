// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Util/P16Type.h"
#include "P16HighlightInterface.generated.h"

UINTERFACE()
class UP16HighlightInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT16_API IP16HighlightInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ToggleHighlight(const bool bOn);
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	EP16TargetingStatus GetTargetingStatus();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void UpdateDestination(FVector& OutDestination);
};
