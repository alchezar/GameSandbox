// Copyright © 2024, Ivan Kinder

#include "Tests/P14GameUserSettings_Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Settings/P14GameUserSettings.h"
#include "Utils/P14Utils.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14SettingsShouldExist, "Project14.GameUserSettings.SettingsShouldExist", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14SettingsShouldExist::RunTest(const FString& Parameters)
{
	AddInfo("Test if game user settings exist.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	const UWorld*          World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	TestTrueExpr(UP14GameUserSettings::Get() != nullptr);
	TestTrueExpr(UP14GameUserSettings::Get()->GetVideoSettings().Num() == 7);

	return true;
}

#endif
