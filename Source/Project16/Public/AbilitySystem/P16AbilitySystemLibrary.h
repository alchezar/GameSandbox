// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Util/P16Type.h"
#include "P16AbilitySystemLibrary.generated.h"

class UP16AttributeMenuWidgetController;
class UP16OverlayWidgetController;

UCLASS()
class PROJECT16_API UP16AbilitySystemLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "C++")
	static UP16OverlayWidgetController* GetOverlayWidgetController(const UObject* WorldContextObject);
	UFUNCTION(BlueprintPure, Category = "C++")
	static UP16AttributeMenuWidgetController* GetAttributeMenuWidgetController(const UObject* WorldContextObject);

private:
	static FP16WidgetControllerParams GetWidgetControllerParams(const UObject* WorldContextObject);
};
