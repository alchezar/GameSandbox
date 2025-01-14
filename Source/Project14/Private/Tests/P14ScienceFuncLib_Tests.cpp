// Copyright © 2024, Ivan Kinder

#include "Tests/P14ScienceFuncLib_Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Science/P14ScienceFuncLib.h"
#include "Utils/P14Utils.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14FibonacciSimple, "Project14.Science.FibonacciSimple", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14FibonacciStress, "Project14.Science.FibonacciStress", P14::Test::TestContext
	| EAutomationTestFlags::StressFilter
	| EAutomationTestFlags::LowPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14FibonacciNegative, "Project14.Science.FibonacciNegative", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

DEFINE_SPEC(FP14Factorial, "Project14.Science.Factorial", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14FibonacciSimple::RunTest(const FString& Parameters)
{
	AddInfo("Fibonacci simple testing");

	TArray<P14::Test::TTestPayload<int32, int32>> TestData = {
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

void FP14Factorial::Define()
{
	Describe("Corner cases", [this]() -> void
	{
		It("Factorial of 0 should return 1", [this]() -> void
		{
			TestTrueExpr(UP14ScienceFuncLib::GetFactorial(0) == 1);
		});
		It("Factorial of 1 should return 1", [this]() -> void
		{
			TestTrueExpr(UP14ScienceFuncLib::GetFactorial(1) == 1);
		});
		It("Factorial of -1 should return -1", [this]() -> void
		{
			TestTrueExpr(UP14ScienceFuncLib::GetFactorial(-1) == -1);
		});
	});

	Describe("Normal cases", [this]() -> void
	{
		TArray<P14::Test::TTestPayload<int32, int32>> TestData = {
			{2, 2},
			{3, 6},
			{4, 24},
			{5, 120},
			{6, 720},
			{7, 5040},
			{8, 40320},
			{9, 362880},
			{10, 3628800}
		};

		for (const P14::Test::TTestPayload<int32, int32>& Data : TestData)
		{
			const FString WhatString = FString::Format(TEXT("Factorial of {0} should return {1}"), {Data.Test, Data.Expected});
			It(WhatString, [this, Data]() -> void
			{
				TestTrueExpr(UP14ScienceFuncLib::GetFactorial(Data.Test) == Data.Expected);
			});
		}
	});
}

#endif
