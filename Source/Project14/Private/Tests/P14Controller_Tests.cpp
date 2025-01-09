// Copyright © 2024, Ivan Kinder

#include "Tests/P14Controller_Tests.h"

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Player/P14Controller.h"
#include "Utils/P14Utils.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14ControllerGameCanBePaused, "Project14.Controller.GameCanBePaused", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14ControllerGameCanBeUnpaused, "Project14.Controller.GameCanBeUnpaused", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14ControllerGameCanBePaused::RunTest(const FString& Parameters)
{
	AddInfo("Test if game can be paused.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	const UWorld*          World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	const AP14Controller* Controller = World->GetFirstPlayerController<AP14Controller>();
	UTEST_NOT_NULL_EXPR(Controller)

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	TFunction<bool()> CheckLambda1 = [this, Controller]() -> bool
	{
		TestTrueExpr(!Controller->IsPaused());
		TestTrueExpr(!Controller->bShowMouseCursor);
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(CheckLambda1)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	TFunction<bool()> ActionLambda = [this, Controller]() -> bool
	{
		P14::Test::InjectActionInput(Controller, "IA_Score", FInputActionValue{true});
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(ActionLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	TFunction<bool()> CheckLambda2 = [this, Controller]() -> bool
	{
		TestTrueExpr(Controller->IsPaused());
		TestTrueExpr(Controller->bShowMouseCursor);
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(CheckLambda2)))

	return true;
}

bool FP14ControllerGameCanBeUnpaused::RunTest(const FString& Parameters)
{
	AddInfo("Test if game can be unpaused.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	const UWorld*          World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	AP14Controller* Controller = World->GetFirstPlayerController<AP14Controller>();
	UTEST_NOT_NULL_EXPR(Controller)

	TFunction<bool()> CheckLambda = [this, Controller]() -> bool
	{
		TestTrueExpr(!Controller->IsPaused());
		TestTrueExpr(!Controller->bShowMouseCursor);
		return true;
	};
	TFunction<bool()> ActionLambda = [this, Controller]() -> bool
	{
		P14::Test::InjectActionInput(Controller, "IA_Score", FInputActionValue{true});
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(CheckLambda))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(ActionLambda))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(2.f))
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(ActionLambda)))
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(CheckLambda)))

	return true;
}

#endif
