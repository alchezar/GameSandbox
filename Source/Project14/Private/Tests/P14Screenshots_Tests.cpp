// Copyright © 2024, Ivan Kinder

#include "Tests/P14Screenshots_Tests.h"

#include "CoreMinimal.h"
#include "Camera/CameraActor.h"
#include "Engine/DamageEvents.h"
#include "Misc/AutomationTest.h"
#include "Utils/P14Utils.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14RenderShouldBeCorrect, "Project14.Screenshots.RenderShouldBeCorrect", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14InterfaceShouldBeRendered, "Project14.Screenshots.InterfaceShouldBeRendered", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14HealthWidgetShouldBeRenderedCorrectly, "Project14.Screenshots.HealthWidgetShouldBeRenderedCorrectly", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14SettingWidgetShouldBeRenderedCorrectlyOnGamePause, "Project14.Screenshots.SettingWidgetShouldBeRenderedCorrectlyOnGamePause", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14RenderShouldBeCorrect::RunTest(const FString& Parameters)
{
	AddInfo("Test if render screenshot is correct.");
	const P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	UWorld*                      World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	// Spawn the new camera.
	const FTransform Transform = {FRotator{-27.6, 59.0, 0.0}, FVector{458.8, -508.3, 454.3}};
	ACameraActor*    Camera    = World->SpawnActor<ACameraActor>(ACameraActor::StaticClass(), Transform);
	UTEST_NOT_NULL_EXPR(Camera)

	// Use the camera as a new view target.
	APlayerController* PlayerController = World->GetFirstPlayerController();
	UTEST_NOT_NULL_EXPR(PlayerController)
	PlayerController->SetViewTarget(Camera);

	ADD_LATENT_AUTOMATION_COMMAND(P14::Test::FTakeGameScreenshotLatentCommand("rendering_check_screenshot"));

	return true;
}

bool FP14InterfaceShouldBeRendered::RunTest(const FString& Parameters)
{
	AddInfo("Test that the interface is displayed correctly.");
	const P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	const UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	ADD_LATENT_AUTOMATION_COMMAND(P14::Test::FTakeGameScreenshotLatentCommand("main_player_hud_screenshot"));

	return true;
}

bool FP14HealthWidgetShouldBeRenderedCorrectly::RunTest(const FString& Parameters)
{
	AddInfo("Test that the health widget is displaying correctly after damage.");
	const P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	const UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	const APlayerController* PlayerController = World->GetFirstPlayerController();
	UTEST_NOT_NULL_EXPR(PlayerController)
	APawn* Pawn = PlayerController->GetPawn();
	UTEST_NOT_NULL_EXPR(Pawn)

	TFunction<bool()> DamageLambda = [this, Pawn]() -> bool
	{
		constexpr float DamageAmount = 99.f;
		Pawn->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(DamageLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))

	ADD_LATENT_AUTOMATION_COMMAND(P14::Test::FTakeUIScreenshotLatentCommand("health_widget_screenshot", NAME_None));

	return true;
}

bool FP14SettingWidgetShouldBeRenderedCorrectlyOnGamePause::RunTest(const FString& Parameters)
{
	AddInfo("Test that the setting widget is displaying correctly.");
	const P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	const UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	APlayerController* PlayerController = World->GetFirstPlayerController();
	UTEST_NOT_NULL_EXPR(PlayerController)

	// Open menu widget.
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	TFunction<bool()> ActionLambda = [this, PlayerController]() -> bool
	{
		P14::Test::InjectActionInput(PlayerController, "IA_Score", FInputActionValue{true});
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(ActionLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(P14::Test::FTakeUIScreenshotLatentCommand("setting_widget_screenshot", "SceneDepth"));

	return true;
}

#endif
