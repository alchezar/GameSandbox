// Copyright © 2024, Ivan Kinder

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PERF_AUTOMATION_TESTS)

#include "Tests/P14InventoryComponent_Tests.h"

#include "CoreMinimal.h"
#include "Components/P14InventoryComponent.h"
#include "Misc/AutomationTest.h"
#include "Tests/P14Utils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14ComponentCouldBeCreated, "Project14.Components.Inventory.ComponentCouldBeCreated", P14::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14ItemScoreShouldBeZeroByDefault, "Project14.Components.Inventory.ItemScoreShouldBeZeroByDefault", P14::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14NegativeItemShouldNotBeAdded, "Project14.Components.Inventory.NegativeItemShouldNotBeAdded", P14::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14PositiveScoreShouldBeAdded, "Project14.Components.Inventory.PositiveScoreShouldBeAdded", P14::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14ScoreAboveLimitShouldNotBeAdded, "Project14.Components.Inventory.ScoreAboveLimitShouldNotBeAdded", P14::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14ComponentCouldBeCreated::RunTest(const FString& Parameters)
{
	AddInfo("Test if component could be created.");
	const UP14InventoryComponent* InventoryComponent = NewObject<UP14InventoryComponent>();
	if (!TestNotNull("Inventory component could be created", InventoryComponent))
	{
		return false;
	}

	return true;
}

bool FP14ItemScoreShouldBeZeroByDefault::RunTest(const FString& Parameters)
{
	AddInfo("Check if item score is zero by default.");
	const UP14InventoryComponent* InventoryComponent = NewObject<UP14InventoryComponent>();

	P14::IterateThroughEnum<EP14InventoryItemType>([this, InventoryComponent](const EP14InventoryItemType InType) -> void
	{
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(InType) == 0);
	});

	return true;
}

bool FP14NegativeItemShouldNotBeAdded::RunTest(const FString& Parameters)
{
	AddInfo("Check if negative item can't be added.");
	UP14InventoryComponent* InventoryComponent = NewObject<UP14InventoryComponent>();

	P14::IterateThroughEnum<EP14InventoryItemType>([this, InventoryComponent](const EP14InventoryItemType InType) -> void
	{
		TestTrueExpr(InventoryComponent->TryAddItem({InType, -1}) == false);
		TestTrueExpr(InventoryComponent->GetInventoryAmountByType(InType) == 0);
	});

	return true;
}

bool FP14PositiveScoreShouldBeAdded::RunTest(const FString& Parameters)
{
	AddInfo("Check if positive score can be added.");
	P14::Test::UP14InventoryComponent_Testable* InventoryComponent = NewObject<P14::Test::UP14InventoryComponent_Testable>();

	constexpr int32 Limit = 100;
	P14::IterateThroughEnum<EP14InventoryItemType>([this, InventoryComponent](const EP14InventoryItemType InType) -> void
	{
		InventoryComponent->SetLimit(InType, Limit);
		for (int32 Index = 0; Index < Limit; ++Index)
		{
			TestTrueExpr(InventoryComponent->TryAddItem({InType, 1}));
			TestTrueExpr(InventoryComponent->GetInventoryAmountByType(InType) == Index + 1);
		}
	});

	return true;
}

bool FP14ScoreAboveLimitShouldNotBeAdded::RunTest(const FString& Parameters)
{
	AddInfo("Check if score above limit can't be added.");
	P14::Test::UP14InventoryComponent_Testable* InventoryComponent = NewObject<P14::Test::UP14InventoryComponent_Testable>();

	constexpr int32 Limit = 100;
	P14::IterateThroughEnum<EP14InventoryItemType>([this, InventoryComponent](const EP14InventoryItemType InType) -> void
	{
		InventoryComponent->SetLimit(InType, Limit);
		TestTrueExpr(InventoryComponent->TryAddItem({InType, Limit}));
		TestTrueExpr(InventoryComponent->TryAddItem({InType, 1}) == false);
	});
	return true;
}

#endif
