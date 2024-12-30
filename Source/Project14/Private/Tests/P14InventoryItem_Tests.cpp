// Copyright © 2024, Ivan Kinder

#if WITH_AUTOMATION_TESTS

#include "Tests/P14InventoryItem_Tests.h"

#include "CoreMinimal.h"
#include "EngineUtils.h"
#include "Components/P14InventoryComponent.h"
#include "Components/SphereComponent.h"
#include "Components/TextRenderComponent.h"
#include "Items/P14InventoryItem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "Player/P14Character.h"
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

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14InventoryDataShouldBeSetupCorrectly, "Project14.Items.Inventory.InventoryDataShouldBeSetupCorrectly", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14InventoryCanBeTaken, "Project14.Items.Inventory.InventoryCanBeTaken", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14AbstractCppActorCouldNotBeCreated::RunTest(const FString& Parameters)
{
	AddInfo("Test if actor could not be created.");

	P14::Test::FLevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld* World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	const FString ExpectedWarning = FString::Format(L"SpawnActor failed because class {0} is abstract", {AP14InventoryItem::StaticClass()->GetName()});
	AddExpectedError(ExpectedWarning, EAutomationExpectedErrorFlags::MatchType::Exact);

	const AP14InventoryItem* Item = World->SpawnActor<AP14InventoryItem>(AP14InventoryItem::StaticClass(), FTransform::Identity);
	UTEST_NULL_EXPR(Item)

	return true;
}

bool FP14PlaceableCppActorShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	AddInfo("Test if collision is setup correctly.");

	P14::Test::FLevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld* World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	const AP14InventoryItem_Test* Item = World->SpawnActor<AP14InventoryItem_Test>(AP14InventoryItem_Test::StaticClass(), FTransform::Identity);
	UTEST_NOT_NULL_EXPR(Item)
	const USphereComponent* CollisionComp = Item->FindComponentByClass<USphereComponent>();
	UTEST_NOT_NULL_EXPR(CollisionComp)

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
	UTEST_NOT_NULL_EXPR(World)

	// Load blueprint and spawn inventory item.
	const FString            BPItemName = "/Script/Engine.Blueprint'/Game/Project/PP14/Items/BP_InventoryItem.BP_InventoryItem'";
	const AP14InventoryItem* Item       = P14::Test::CreateBlueprint<AP14InventoryItem>(World, BPItemName);
	UTEST_NOT_NULL_EXPR(Item)

	// Collision tests.
	const USphereComponent* CollisionComp = Item->FindComponentByClass<USphereComponent>();
	UTEST_NOT_NULL_EXPR(CollisionComp)
	TestTrueExpr(CollisionComp->GetCollisionEnabled() == ECollisionEnabled::Type::QueryOnly);
	TestTrueExpr(CollisionComp->GetGenerateOverlapEvents());
	P14::Test::IterateThroughEnum(ECC_EngineTraceChannel1, [this, CollisionComp](const ECollisionChannel InChannel)-> void
	{
		TestTrueExpr(CollisionComp->GetCollisionResponseToChannel(InChannel) == ECollisionResponse::ECR_Overlap);
	});
	TestTrueExpr(Item->GetRootComponent() == CollisionComp);

	// Components tests.
	const UTextRenderComponent* TextComp = Item->FindComponentByClass<UTextRenderComponent>();
	UTEST_NOT_NULL_EXPR(TextComp)
	const UStaticMeshComponent* MeshComp = Item->FindComponentByClass<UStaticMeshComponent>();
	UTEST_NOT_NULL_EXPR(MeshComp)
	TestTrueExpr(MeshComp->GetCollisionEnabled() == ECollisionEnabled::Type::NoCollision);

	return true;
}

bool FP14InventoryDataShouldBeSetupCorrectly::RunTest(const FString& Parameters)
{
	AddInfo("Test if inventory data is setup correctly.");

	P14::Test::FLevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld* World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	const FString      BPTestItemName = "/Script/Engine.Blueprint'/Game/Project/PP14/Tests/Items/BP_InventoryItemChild_Test.BP_InventoryItemChild_Test'";
	AP14InventoryItem* Item           = P14::Test::CreateBlueprint<AP14InventoryItem>(World, BPTestItemName);
	UTEST_NOT_NULL_EXPR(Item)

	P14::Test::CallFuncByNameWithParams(Item, "SetTestData", {
		FString::FromInt(123),
		FString::SanitizeFloat(345.67f),
		FString{"\"Test string\""},
		FString{"\"true\""},
		FString::Printf(L"(X=%f Y=%f Z=%f)", 120.f, 130.f, 140.f)
	});

	constexpr FP14InventoryData TestData  = {EP14InventoryItemType::Sphere, 13};
	const FLinearColor          TestColor = {FLinearColor::Yellow};
	P14::Test::CallFuncByNameWithParams(Item, "SetInventoryData", {TestData.ToString(), TestColor.ToString()});
	TestTrueExpr(Item->GetInventoryData() == TestData);

	const UTextRenderComponent* TextComp = Item->FindComponentByClass<UTextRenderComponent>();
	UTEST_NOT_NULL_EXPR(TextComp)
	TestTrueExpr(TextComp->Text.ToString().Equals(FString::FromInt(TestData.Score)));

	const UStaticMeshComponent* MeshComp = Item->FindComponentByClass<UStaticMeshComponent>();
	UTEST_NOT_NULL_EXPR(Item)
	TestTrueExpr(MeshComp->GetStaticMesh() && MeshComp->GetStaticMesh() == Item->GetMeshesMap()[EP14InventoryItemType::Sphere]);

	const UMaterialInterface* Material = MeshComp->GetMaterial(0);
	UTEST_NOT_NULL_EXPR(Material)
	FLinearColor CurrentColor = {};
	Material->GetVectorParameterValue({"Color"}, CurrentColor);
	TestTrueExpr(CurrentColor == TestColor);

	return true;
}

bool FP14InventoryCanBeTaken::RunTest(const FString& Parameters)
{
	AddInfo("Test if inventory can be taken.");

	P14::Test::FLevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld* World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	const FTransform   InitTransform{FVector{1000.f}};
	const FString      BPTestItemName = "/Script/Engine.Blueprint'/Game/Project/PP14/Tests/Items/BP_InventoryItemChild_Test.BP_InventoryItemChild_Test'";
	AP14InventoryItem* Item           = P14::Test::CreateBlueprint<AP14InventoryItem>(World, BPTestItemName, InitTransform);
	UTEST_NOT_NULL_EXPR(Item)

	constexpr FP14InventoryData TestData{EP14InventoryItemType::Sphere, 13};
	const FLinearColor          TestColor{FLinearColor::Yellow};
	P14::Test::CallFuncByNameWithParams(Item, "SetInventoryData", {TestData.ToString(), TestColor.ToString()});

	AP14Character* Char = nullptr;
	for (TActorIterator<AP14Character> Iterator{World}; Iterator; ++Iterator)
	{
		if (*Iterator)
		{
			Char = *Iterator;
			break;
		}
	}
	UTEST_NOT_NULL_EXPR(Char)

	const UP14InventoryComponent* InvComp = Char->FindComponentByClass<UP14InventoryComponent>();
	UTEST_NOT_NULL_EXPR(InvComp)
	TestTrueExpr(InvComp->GetInventoryAmountByType(TestData.Type) == 0);
	Char->SetActorLocation(InitTransform.GetLocation());
	TestTrueExpr(InvComp->GetInventoryAmountByType(TestData.Type) == TestData.Score);
	TestTrueExpr(!IsValid(Item));

	TArray<AActor*> InvComps = {};
	UGameplayStatics::GetAllActorsOfClass(World, AP14InventoryItem::StaticClass(), InvComps);
	TestTrueExpr(InvComps.IsEmpty());

	return true;
}

#endif
