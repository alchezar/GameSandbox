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

int32 UP14ScienceFuncLib::GetFactorial(const int32 Value)
{
	if (Value < 0)
	{
		return -1;
	}
#if 0
	if (Value <= 1)
	{
		return 1;
	}
	return Value * GetFactorial(Value - 1);
#endif

	int32 Fac = 1;
	for (int32 Index = 1; Index <= Value; ++Index)
	{
		Fac *= Index;
	}

	return Fac;
}
