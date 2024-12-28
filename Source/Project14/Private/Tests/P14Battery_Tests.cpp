// Copyright © 2024, Ivan Kinder

#if WITH_AUTOMATION_TESTS

#include "Tests/P14Battery_Tests.h"

#include "CoreMinimal.h"
#include "Items/P14Battery.h"
#include "Misc/AutomationTest.h"
#include "Tests/P14Utils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14BatteryTests, "Project14.Items.Battery", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14BatteryTests::RunTest(const FString& Parameters)
{
	AddInfo("Battery with default constructor.");
	constexpr P14::FBattery BatteryDefault{};
	UTEST_TRUE_EXPR(FMath::IsNearlyEqual(BatteryDefault.GetPercent(), 1.f));
	UTEST_TRUE_EXPR(BatteryDefault.GetColor() == FColor::Green);
	UTEST_TRUE_EXPR(BatteryDefault.ToString() == TEXT("100%"));

	AddInfo("Battery with custom constructors.");
	const auto BatteryTestFunc = [this](const float InPercent, const FColor& InColor, const FString& InString) -> bool
	{
		const P14::FBattery BatteryTest{InPercent};
		UTEST_TRUE_EXPR(FMath::IsNearlyEqual(BatteryTest.GetPercent(), FMath::Clamp(InPercent, 0.f, 1.f)));
		UTEST_TRUE_EXPR(BatteryTest.GetColor() == InColor);
		UTEST_TRUE_EXPR(BatteryTest.ToString() == InString);

		return true;
	};

	BatteryTestFunc(1.f, FColor::Green, "100%");
	BatteryTestFunc(0.46f, FColor::Yellow, "46%");
	BatteryTestFunc(0.13f, FColor::Red, "13%");
	BatteryTestFunc(3000.f, FColor::Green, "100%");
	BatteryTestFunc(-3000.f, FColor::Red, "0%");

	AddInfo("Battery charge/discharge.");
	P14::FBattery BatteryObject{0.6f};
	UTEST_TRUE_EXPR(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));
	BatteryObject.Discharge();
	UTEST_TRUE_EXPR(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.5f));
	BatteryObject.Charge();
	UTEST_TRUE_EXPR(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.6f));

	AddInfo("Battery charge/discharge.");
	for (int32 Index = 0; Index < 100; ++Index)
	{
		BatteryObject.Discharge();
	}
	UTEST_TRUE_EXPR(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 0.f));
	for (int32 Index = 0; Index < 100; ++Index)
	{
		BatteryObject.Charge();
	}
	UTEST_TRUE_EXPR(FMath::IsNearlyEqual(BatteryObject.GetPercent(), 1.f));

	AddInfo("Battery comparison.");
	const P14::FBattery BatteryHigh{1.f};
	const P14::FBattery BatteryLow{0.3f};

	UTEST_TRUE_EXPR(BatteryHigh >= BatteryLow);
	UTEST_TRUE_EXPR(FMath::Max(BatteryHigh, BatteryLow) == BatteryHigh);

	AddInfo("Battery memory comparison.");
	const P14::FBattery& BatteryRef = BatteryLow;
	UTEST_NOT_SAME_EXPR(BatteryHigh, BatteryDefault);
	UTEST_SAME_EXPR(BatteryLow, BatteryRef);

	return true;
}

#endif
