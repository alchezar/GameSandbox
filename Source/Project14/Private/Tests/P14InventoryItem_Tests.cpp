// Copyright © 2024, Ivan Kinder

#if WITH_AUTOMATION_TESTS

#include "Tests/P14InventoryItem_Tests.h"

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Items/P14InventoryItem.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Tests/P14Utils.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14AbstractCppActorCouldNotBeCreated, "Project14.Items.Inventory.AbstractCppActorCouldNotBeCreated", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14PlaceableCppActorShouldBeSetupCorrectly, "Project14.Items.Inventory.PlaceableCppActorShouldBeSetupCorrectly", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14BlueprintShouldBeSetupCorrectly, "Project14.Items.Inventory.BlueprintShouldBeSetupCorrectly", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14AbstractCppActorCouldNotBeCreated::RunTest(const FString& Parameters)
{
	AddInfo("Test if actor could not be created.");

	P14::Test::FLevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld* World = AutomationCommon::GetAnyGameWorld();

	if (!TestNotNull("World exists", World))
	{
		return false;
	}

	const FString ExpectedWarning = FString::Format(L"SpawnActor failed because class {0} is abstract", {AP14InventoryItem::StaticClass()->GetName()});
	AddExpectedError(ExpectedWarning, EAutomationExpectedErrorFlags::MatchType::Exact);

	const AP14InventoryItem* Item = World->SpawnActor<AP14InventoryItem>(AP14InventoryItem::StaticClass(), FTransform::Identity);
	if (!TestNull("Inventory item exists", Item))
	{
		return false;
	}

	return true;
}

bool FP14PlaceableCppActorShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	AddInfo("Test if collision is setup correctly.");

	P14::Test::FLevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld* World = AutomationCommon::GetAnyGameWorld();

	if (!TestNotNull("World exists", World))
	{
		return false;
	}

	const AP14InventoryItem_Test* Item = World->SpawnActor<AP14InventoryItem_Test>(AP14InventoryItem_Test::StaticClass(), FTransform::Identity);
	if (!TestNotNull("Inventory item exists", Item))
	{
		return false;
	}

	const USphereComponent* CollisionComp = Item->FindComponentByClass<USphereComponent>();
	if (!TestNotNull("Collision component exists", CollisionComp))
	{
		return false;
	}

	TestTrueExpr(FMath::IsNearlyEqual(CollisionComp->GetUnscaledSphereRadius(), Item->GetRadius()));
	TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::Type::QueryOnly);
	TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
	P14::Test::IterateThroughEnum(ECC_EngineTraceChannel1, [this, CollisionComp](const ECollisionChannel InChannel)-> void
	{
		TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(InChannel) == ECollisionResponse::ECR_Overlap);
	});
	TestTrueExpr(Item->GetRootComponent() == CollisionComp);

	return true;
}

bool FP14BlueprintShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	AddInfo("Test if blueprint could be loaded.");

	// Open level and get the world.
	P14::Test::FLevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld* World = AutomationCommon::GetAnyGameWorld();

	// Load blueprint and spawn inventory item.
	const FString            BPItemName = "/Script/Engine.Blueprint'/Game/Project/PP14/Items/BP_InventoryItem.BP_InventoryItem'";
	const AP14InventoryItem* Item       = P14::Test::CreateBlueprint<AP14InventoryItem>(World, BPItemName);
	if (!TestNotNull("Inventory item exists", Item))
	{
		return false;
	}

	// Collision tests.
	const USphereComponent* CollisionComp = Item->FindComponentByClass<USphereComponent>();
	if (!TestNotNull("Collision component exists", CollisionComp))
	{
		return false;
	}

	TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::Type::QueryOnly);
	TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
	P14::Test::IterateThroughEnum(ECC_EngineTraceChannel1, [this, CollisionComp](const ECollisionChannel InChannel)-> void
	{
		TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(InChannel) == ECollisionResponse::ECR_Overlap);
	});
	TestTrueExpr(Item->GetRootComponent() == CollisionComp);

	// Components tests.
	const UTextRenderComponent* TextComp = Item->FindComponentByClass<UTextRenderComponent>();
	if (!TestNotNull("Text renderer exists", TextComp))
	{
		return false;
	}
	const UStaticMeshComponent* MeshComp = Item->FindComponentByClass<UStaticMeshComponent>();
	if (!TestNotNull("Mesh exists", MeshComp))
	{
		return false;
	}
	TestTrueExpr(MeshComp->GetCollisionEnabled() == ECollisionEnabled::Type::NoCollision);

	return true;
}

#endif
