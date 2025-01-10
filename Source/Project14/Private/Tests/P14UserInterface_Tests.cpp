// Copyright © 2024, Ivan Kinder

#include "Tests/P14UserInterface_Tests.h"

#include "CoreMinimal.h"
#include "Components/Button.h"
#include "Components/VerticalBox.h"
#include "Misc/AutomationTest.h"
#include "Settings/P14GameUserSettings.h"
#include "UI/P14PauseMenu.h"
#include "UI/P14VideoSettings.h"
#include "Utils/P14Utils.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14PauseMenuShouldBeVisibleOnGamePaused, "Project14.UI.PauseMenuShouldBeVisibleOnGamePaused", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14PauseMenuShouldBeCollapsedOnGameUnpaused, "Project14.UI.PauseMenuShouldBeCollapsedOnGameUnpaused", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14AllVideoSettingsExist, "Project14.UI.AllVideoSettingsExist", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14SettingCanBeApplied, "Project14.UI.SettingCanBeApplied", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14AutoBenchmarkShouldWork, "Project14.UI.AutoBenchmarkShouldWork", P14::Test::TestContext
	| EAutomationTestFlags::StressFilter
	| EAutomationTestFlags::MediumPriority)

bool FP14PauseMenuShouldBeVisibleOnGamePaused::RunTest(const FString& Parameters)
{
	AddInfo("Test if pause menu should be visible on game paused.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World);

	const APlayerController* Controller = World->GetFirstPlayerController();
	UTEST_NOT_NULL_EXPR(Controller)

	UP14PauseMenu* PauseMenuWidget = P14::Test::FindWidgetByClass<UP14PauseMenu>(World);

	// Check if menu is hidden.
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	TFunction<bool()> CheckLambda1 = [this, PauseMenuWidget]() -> bool
	{
		TestTrueExpr(PauseMenuWidget != nullptr);
		TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(CheckLambda1)))

	// Open menu widget.
	TFunction<bool()> ActionLambda = [this, Controller]() -> bool
	{
		P14::Test::InjectActionInput(Controller, "IA_Score", FInputActionValue{true});
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(ActionLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))

	// Check if menu is visible.
	TFunction<bool()> CheckLambda2 = [this, PauseMenuWidget]() -> bool
	{
		TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Visible);
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(CheckLambda2)))

	return true;
}

bool FP14PauseMenuShouldBeCollapsedOnGameUnpaused::RunTest(const FString& Parameters)
{
	AddInfo("Test if pause menu should be collapsed on game unpaused.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World);

	const APlayerController* Controller = World->GetFirstPlayerController();
	UTEST_NOT_NULL_EXPR(Controller)

	UP14PauseMenu* PauseMenuWidget = P14::Test::FindWidgetByClass<UP14PauseMenu>(World);

	// Check if menu is hidden.
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	TFunction<bool()> CheckLambda = [this, PauseMenuWidget]() -> bool
	{
		TestTrueExpr(PauseMenuWidget != nullptr);
		TestTrueExpr(PauseMenuWidget->GetVisibility() == ESlateVisibility::Collapsed);
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(CheckLambda))

	// Open and close menu.
	TFunction<bool()> ActionLambda = [this, Controller]() -> bool
	{
		P14::Test::InjectActionInput(Controller, "IA_Score", FInputActionValue{true});
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(ActionLambda))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(ActionLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))

	// Check if menu is still hidden.
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(CheckLambda)))

	return true;
}

bool FP14AllVideoSettingsExist::RunTest(const FString& Parameters)
{
	AddInfo("Test if all video settings exist.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World);

	const UP14VideoSettings* VideoSettingsWidget = P14::Test::FindWidgetByClass<UP14VideoSettings>(World);
	UTEST_NOT_NULL_EXPR(VideoSettingsWidget);
	const UButton* BenchmarkButton = P14::Test::FindWidgetChildByName<UButton>(VideoSettingsWidget, "RunBenchmarkButton");
	UTEST_NOT_NULL_EXPR(BenchmarkButton);
	const UVerticalBox* VerticalBox = P14::Test::FindWidgetChildByName<UVerticalBox>(VideoSettingsWidget, "VideoSettingsContainer");
	UTEST_NOT_NULL_EXPR(VerticalBox);

	TestTrueExpr(VerticalBox->GetChildrenCount() > 0);
	TestTrueExpr(VerticalBox->GetChildrenCount() == UP14GameUserSettings::Get()->GetVideoSettings().Num());

	return true;
}

bool FP14SettingCanBeApplied::RunTest(const FString& Parameters)
{
	AddInfo("Test if video settings can be applied.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World);
	const APlayerController* Controller = World->GetFirstPlayerController();
	UTEST_NOT_NULL_EXPR(Controller)

	const UP14VideoSettings* VideoSettingsWidget = P14::Test::FindWidgetByClass<UP14VideoSettings>(World);
	UTEST_NOT_NULL_EXPR(VideoSettingsWidget)
	const UVerticalBox* VerticalBox = P14::Test::FindWidgetChildByName<UVerticalBox>(VideoSettingsWidget, "VideoSettingsContainer");
	UTEST_NOT_NULL_EXPR(VerticalBox)

	// Save GI quality for further tests.
	const int32 GIQuality = UP14GameUserSettings::Get()->GetGlobalIlluminationQuality();
	TestTrueExpr(GIQuality == UP14GameUserSettings::Get()->GetVideoSettings()[2]->GetCurrentValue());
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))

	// Open pause menu.
	TFunction<bool()> PauseLambda = [this, Controller]() -> bool
	{
		P14::Test::InjectActionInput(Controller, "IA_Score", FInputActionValue{true});
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(PauseLambda)))

	// Click |>| on each setting.
	for (int32 Index = 0; Index < VerticalBox->GetChildrenCount(); ++Index)
	{
		ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
		TFunction<bool()> SettingLambda = [this, World, Index]() -> bool
		{
			P14::Test::DoNextSettingClick(World, Index);
			return true;
		};
		ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(SettingLambda)))
	}
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))

	// Check if the GI quality has changed.
	TFunction<bool()> CheckLambda = [this, GIQuality]() -> bool
	{
		TestTrueExpr(GIQuality != UP14GameUserSettings::Get()->GetVideoSettings()[2]->GetCurrentValue());
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(CheckLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))

	// Click |<| on each setting to match the pre-test state.
	TFunction<bool()> ResetLambda = [this]() -> bool
	{
		for (const UP14GameSetting* Setting : UP14GameUserSettings::Get()->GetVideoSettings())
		{
			Setting->UpdateCurrentValue(-1);
		}
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(ResetLambda)))

	return true;
}

bool FP14AutoBenchmarkShouldWork::RunTest(const FString& Parameters)
{
	AddInfo("Test if auto benchmark is working.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World);

	const APlayerController* Controller = World->GetFirstPlayerController();
	UTEST_NOT_NULL_EXPR(Controller)

	// Open pause menu.
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	TFunction<bool()> ActionLambda = [this, Controller]() -> bool
	{
		P14::Test::InjectActionInput(Controller, "IA_Score", FInputActionValue{true});
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(ActionLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))

	// Test the benchmark button.
	TFunction<bool()> UpdateLambda = [this, World]() -> bool
	{
		P14::Test::DoBenchmarkSettingClick(World);

		TArray<int32> SavedSettingsValues = {};
		for (int32 Index = 0; Index < SavedSettingsValues.Num(); ++Index)
		{
			const UP14GameSetting* Setting = UP14GameUserSettings::Get()->GetVideoSettings()[Index];

			SavedSettingsValues.Add(Setting->GetCurrentValue());
			Setting->UpdateCurrentValue(1);
			TestTrueExpr(SavedSettingsValues[Index] != Setting->GetCurrentValue());
		}

		P14::Test::DoBenchmarkSettingClick(World);

		for (int32 Index = 0; Index < SavedSettingsValues.Num(); ++Index)
		{
			TestTrueExpr(SavedSettingsValues[Index] == UP14GameUserSettings::Get()->GetVideoSettings()[Index]->GetCurrentValue());
		}
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(UpdateLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))

	return true;
}

#endif
