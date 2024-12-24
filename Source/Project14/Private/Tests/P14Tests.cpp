// Copyright © 2024, Ivan Kinder

#include "Tests/P14Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14MathMaxInt, "Project14.Math.MaxInt", EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ProgramContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14MathSqrt, "Project14.Math.Sqrt", EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ProgramContext | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority)

bool FP14MathMaxInt::RunTest(const FString& Parameters)
{
	AddInfo("Max [int] func testing");

	TestTrueExpr(FMath::Max(0, 123) == 123);
	TestEqual("Two equal positive numbers", FMath::Max(25, 25), 25);
	TestTrue("Two different positive numbers", FMath::Max(13, 25) == 25);
	TestTrue("Two zeroes", FMath::Max(0, 0) == 0);
	TestTrue("Negative number and zero", FMath::Max(-24, 0) == 0);
	TestTrue("Two different negative numbers", FMath::Max(-45, -67) == -45);
	TestTrue("Two equal negative numbers", FMath::Max(-9, -9) == -9);

	return true;
}

bool FP14MathSqrt::RunTest(const FString& Parameters)
{
	AddInfo("Square root func testing");

	TestEqual("Sqrt(4) [0]", FMath::Sqrt(4.f), 2.f);
	TestEqual("Sqrt(3) [1]", FMath::Sqrt(3.f), 1.73f, 0.01f);

	return true;
}
