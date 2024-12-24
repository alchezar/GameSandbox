// Copyright © 2024, Ivan Kinder

#include "Science/P14ScienceFuncLib.h"

DEFINE_LOG_CATEGORY_STATIC(LogP14Science, All, All);

int32 UP14ScienceFuncLib::GetFibonacci(const int32 Value)
{
	if (Value < 0)
	{
		UE_LOG(LogP14Science, Error, TEXT("Invalid input for Fibonacci: %i"), Value);
	}

	if (Value <= 1)
	{
		return Value;
	}

	return GetFibonacci(Value - 1) + GetFibonacci(Value - 2);
}
