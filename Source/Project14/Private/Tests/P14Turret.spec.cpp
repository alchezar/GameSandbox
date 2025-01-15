// Copyright © 2024, Ivan Kinder

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Utils/P14Utils.h"
#include "Weapon/P14Turret.h"

#if WITH_AUTOMATION_TESTS

namespace P14::Test::Turret
{
	inline const FString MapName       = "/Game/Project/PP14/Level/P14_TestEmpty";
	inline const FString BlueprintName = "/Script/Engine.Blueprint'/Game/Project/PP14/Tests/BP_Turret_Test_P14.BP_Turret_Test_P14'";

	inline const FTransform InitTransform{FVector{0.f, 200.f, 0.f}};
}

#if 0
DEFINE_SPEC(FP14TestClassName, "Project14.Category.TestName", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)
#endif

BEGIN_DEFINE_SPEC(FP14TurretTest, "Project14.Turret", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)
	UWorld*     World  = nullptr;
	AP14Turret* Turret = nullptr;
END_DEFINE_SPEC(FP14TurretTest)

void FP14TurretTest::Define()
{
	Describe("Creation", [this]() -> void
	{
		BeforeEach([this]() -> void
		{
			AutomationOpenMap(P14::Test::Turret::MapName);
			World = AutomationCommon::GetAnyGameWorld();
			TestNotNull("World exists", World);
		});

		It("Cpp instance cant be created", [this]() -> void
		{
			const FString ExpectedWarning = FString::Format(L"SpawnActor failed because class {0} is abstract", {AP14Turret::StaticClass()->GetName()});
			AddExpectedError(ExpectedWarning, EAutomationExpectedErrorFlags::MatchType::Exact);

			Turret = World->SpawnActor<AP14Turret>(AP14Turret::StaticClass(), P14::Test::Turret::InitTransform);
			TestNull("Turret doesn't exists", Turret);
		});

		It("Blueprint instance can be created", [this]() -> void
		{
			Turret = P14::Test::CreateBlueprint<AP14Turret>(World, P14::Test::Turret::BlueprintName, P14::Test::Turret::InitTransform);
			TestNotNull("Turret exists", Turret);
		});

		AfterEach([this]() -> void
		{
			P14::Test::AutomationCloseMap(World);
		});
	});

	Describe("Defaults", [this]() -> void
	{
		BeforeEach([this]() -> void
		{
			AutomationOpenMap(P14::Test::Turret::MapName);
			World = AutomationCommon::GetAnyGameWorld();
			TestNotNull("World exists", World);

			Turret = P14::Test::CreateBlueprint<AP14Turret>(World, P14::Test::Turret::BlueprintName, P14::Test::Turret::InitTransform);
			TestNotNull("Turret exists", Turret);
		});

		const TArray<TTuple<int32, float>> TestData = {{45, 2}};
		for (const auto& [Ammo, Freq] : TestData)
		{
			const FString TestName = FString::Format(L"Ammo: {0} and Frequency: {1}", {Ammo, StaticCast<int32>(Freq)});
			It(TestName, [this, Ammo, Freq]() -> void
			{
				P14::Test::CallBlueprintFuncByNameWithParams(Turret, "SetTurretData", {FString::FromInt(Ammo), FString::SanitizeFloat(Freq)});
				const int32 AmmoCount = P14::Test::GetPropertyValueByName<int32>(Turret, "AmmoCount");
				TestTrueExpr(AmmoCount == Ammo);
				const float Frequency = P14::Test::GetPropertyValueByName<float>(Turret, "FireFrequency");
				TestTrueExpr(Frequency == Freq);
			});
		}

		AfterEach([this]() -> void
		{
			P14::Test::AutomationCloseMap(World);
		});
	});

	Describe("Ammo", [this]() -> void
	{
		constexpr int32 InitAmmoCount = 5;
		constexpr float InitFrequency = 1.f;

		LatentBeforeEach([this, InitAmmoCount, InitFrequency](const FDoneDelegate& TestDone) -> void
		{
			AutomationOpenMap(P14::Test::Turret::MapName);
			World = AutomationCommon::GetAnyGameWorld();
			TestNotNull("World exists", World);

			Turret = P14::Test::CreateBlueprint<AP14Turret>(World, P14::Test::Turret::BlueprintName, P14::Test::Turret::InitTransform);
			TestNotNull("Turret exists", Turret);

			P14::Test::CallBlueprintFuncByNameWithParams(Turret, "SetTurretData", {FString::FromInt(InitAmmoCount), FString::SanitizeFloat(InitFrequency)});
			TestDone.Execute();
		});

		const FString TestName = FString::Format(L"Should be empty after {0} sec", {FMath::RoundToInt(InitAmmoCount * InitFrequency)});
		LatentIt(TestName, EAsyncExecution::ThreadPool, [this, InitAmmoCount, InitFrequency](const FDoneDelegate& TestDone) -> void
		{
			AsyncTask(ENamedThreads::GameThread, [this]() -> void
			{
				const int32 AmmoCount = P14::Test::GetPropertyValueByName<int32>(Turret, "AmmoCount");
				TestTrueExpr(AmmoCount == InitAmmoCount);

				const FTimerHandle FireTimer = P14::Test::GetPropertyValueByName<FTimerHandle>(Turret, "FireTimer");
				TestTrueExpr(World->GetTimerManager().IsTimerActive(FireTimer));
			});

			constexpr float SyncDelta = 0.5f;
			FPlatformProcess::Sleep(InitAmmoCount * InitFrequency + SyncDelta);

			AsyncTask(ENamedThreads::GameThread, [this]() -> void
			{
				const int32 AmmoCount = P14::Test::GetPropertyValueByName<int32>(Turret, "AmmoCount");
				TestTrueExpr(AmmoCount == 0);

				const FTimerHandle FireTimer = P14::Test::GetPropertyValueByName<FTimerHandle>(Turret, "FireTimer");
				TestTrueExpr(!World->GetTimerManager().IsTimerActive(FireTimer));
			});

			TestDone.Execute();
		});

		LatentAfterEach([this](const FDoneDelegate& TestDone) -> void
		{
			P14::Test::AutomationCloseMap(World);
			TestDone.Execute();
		});
	});
}

#endif
