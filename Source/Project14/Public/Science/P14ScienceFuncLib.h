// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"

#include "P14ScienceFuncLib.generated.h"

UCLASS()
class PROJECT14_API UP14ScienceFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Science")
	static int32 GetFibonacci(const int32 Value);
};
