// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P16PlayerInterface.generated.h"

UINTERFACE()
class UP16PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT16_API IP16PlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(int32 XP);
};
