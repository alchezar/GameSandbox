// Copyright © 2024, Ivan Kinder

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/P14Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/P14Utils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14MathMaxInt, "Project14.Math.MaxInt", EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ProgramContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14MathSqrt, "Project14.Math.Sqrt", EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ProgramContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14MathSin, "Project14.Math.Sin", EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ProgramContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

bool FP14MathMaxInt::RunTest(const FString& Parameters)
{
	AddInfo("Max [int] func testing");

	TArray<P14::TTestPayload<TInterval<int32>, int32>> TestData = {
		{{0, 123}, 123},
		{{25, 25}, 25},
		{{13, 25}, 25},
		{{0, 0}, 0},
		{{-24, 0}, 0},
		{{-45, -67}, -45},
		{{-9, -9}, -9},
		{{-78, 34}, 34}
	};

	for (const auto& Data : TestData)
	{
		const FString WhatString = FString::Format(TEXT("Max value between {0} and {1} should be {2}"),
			{Data.Test.Min, Data.Test.Max, Data.Expected});
		TestTrue(WhatString, FMath::Max(Data.Test.Min, Data.Test.Max) == Data.Expected);
	}

	return true;
}

bool FP14MathSqrt::RunTest(const FString& Parameters)
{
	AddInfo("Square root func testing");

	TArray<P14::TTestPayload<float, float>> TestData = {
		{4.f, 2.f},
		{3.f, 1.7f, 0.1f},
		{3.f, 1.73f, 0.01f},
		{3.f, 1.73205f, 1.e-5f}
	};

	for (const auto& Data : TestData)
	{
		const FString WhatString = FString::Format(TEXT("Sqrt {0} == {1}"), {Data.Test, Data.Expected});
		TestEqual(WhatString, FMath::Sqrt(Data.Test), Data.Expected, Data.Tolerance);
	}

	return true;
}

bool FP14MathSin::RunTest(const FString& Parameters)
{
	AddInfo("Sinus func testing");

	TArray<P14::TTestPayload<float, float>> TestData = {
		{0.f, 0.f},
		{PI / 2.f, 1.f},
		{PI, 0.f},
		{3.f * PI / 2.f, -1.f},
		{2.f * PI, 0.f}
	};

	for (const auto& Data : TestData)
	{
		const FString WhatString = FString::Format(TEXT("Sin({0}) == {1}"), {Data.Test, Data.Expected});
		TestEqual(WhatString, FMath::Sin(Data.Test), Data.Expected);
	}

	return true;
}

#endif
