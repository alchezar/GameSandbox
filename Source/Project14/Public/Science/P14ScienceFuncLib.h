// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"

#include "P14ScienceFuncLib.generated.h"

UCLASS()
class PROJECT14_API UP14ScienceFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/// Calculates Fibonacci number for the input number.
	/// https://en.wikipedia.org/wiki/Fibonacci_number
	/// @param Value Number for which Fibonacci should be calculated.
	/// @return Fibonacci number or -Value for negative numbers.
	UFUNCTION(BlueprintCallable, Category = "Science")
	static int32 GetFibonacci(const int32 Value);

	/// Calculates factorial for the input number.
	/// https://en.wikipedia.org/wiki/Factorial
	/// @param Value Number for which factorial should be calculated.
	/// @return Factorial or -1 for negative numbers.
	UFUNCTION(BlueprintCallable, Category = "Science")
	static int32 GetFactorial(const int32 Value);
};
