// Copyright © 2024, Ivan Kinder

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/P14ScienceFuncLib_Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Science/P14ScienceFuncLib.h"
#include "Tests/P14Utils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14FibonacciSimple, "Project14.Science.FibonacciSimple", P14::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14FibonacciStress, "Project14.Science.FibonacciStress", P14::TestContext
	| EAutomationTestFlags::StressFilter
	| EAutomationTestFlags::LowPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14FibonacciNegative, "Project14.Science.FibonacciNegative", P14::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14FibonacciSimple::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci simple testing");

	TArray<P14::TTestPayload<int32, int32>> TestData = {
		{0, 0},
		{1, 1},
		{2, 1},
		{3, 2},
		{4, 3},
		{5, 5}
	};

	for (const auto& Data : TestData)
	{
		const FString WhatString = FString::Format(TEXT("Testing fibonacci {0} value = {1}"), {Data.Test, Data.Expected});
		TestEqual(WhatString, UP14ScienceFuncLib::GetFibonacci(Data.Test), Data.Expected);
	}

	return true;
}

bool FP14FibonacciStress::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci stress testing");

	int32 PrevPrev = 0;
	int32 Prev     = 1;
	for (int32 Index = 2; Index < 40; ++Index)
	{
		const int32 Current = UP14ScienceFuncLib::GetFibonacci(Index);

		const FString WhatString = FString::Format(TEXT("Stress testing fibonacci {0}"), {Index});
		TestTrue(WhatString, Current == (Prev + PrevPrev));

		PrevPrev = Prev;
		Prev     = Current;
	}

	return true;
}

bool FP14FibonacciNegative::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci negative number on input produces error");

	AddExpectedError("Invalid input for Fibonacci", EAutomationExpectedErrorFlags::MatchType::Contains);
	UP14ScienceFuncLib::GetFibonacci(-10);

	return true;
}

#endif
