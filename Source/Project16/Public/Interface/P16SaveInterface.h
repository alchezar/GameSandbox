// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P16SaveInterface.generated.h"

UINTERFACE()
class UP16SaveInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT16_API IP16SaveInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	bool GetShouldLoadTransform();
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void LoadActor();
};
