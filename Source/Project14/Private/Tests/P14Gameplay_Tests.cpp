// Copyright © 2025, Ivan Kinder

#include "Tests/P14Gameplay_Tests.h"

#include "CoreMinimal.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameFramework/Character.h"
#include "Items/P14InventoryItem.h"
#include "Kismet/GameplayStatics.h"
#include "Misc/AutomationTest.h"
#include "Utils/P14JsonUtils.h"
#include "Utils/P14Utils.h"

#if WITH_AUTOMATION_TESTS

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14GameplayInventoryItemCanBeTakenOnJump, "Project14.Gameplay.ItemCanBeTakenOnJump", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14GameplayInventoryItemCanNotBeTakenOnJumpIfTooHigh, "Project14.Gameplay.ItemCanNotBeTakenOnJumpIfTooHigh", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14GameplayAllItemsCanBeTakenOnMovement, "Project14.Gameplay.AllItemsCanBeTakenOnMovement", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14GameplayAllItemsCanBeTakenOnRecordedMovement, "Project14.Gameplay.AllItemsCanBeTakenOnRecordedMovement", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_COMPLEX_AUTOMATION_TEST(FP14GameplayAllMapsShouldLoads, "Project14.Gameplay.AllMapsShouldLoads", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

/* * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Custom class of the latent commands by using macro. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * */

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FP14JumpLatentCommand, ACharacter*, Char);

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FP14CheckItemsAmountLatentCommand, UWorld*, World, int32, ItemsAmount);

bool FP14JumpLatentCommand::Update()
{
	if (!Char || !Char->InputComponent)
	{
		return true;
	}

	const UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(Char->InputComponent);
	check(EnhancedInputComp)
	const int32 JumpActionIndex = P14::Test::GetActionBindingIndex(EnhancedInputComp, "IA_Jump", ETriggerEvent::Started);
	check(JumpActionIndex != INDEX_NONE);
	const TUniquePtr<FEnhancedInputActionEventBinding>& ActionBinding = EnhancedInputComp->GetActionEventBindings()[JumpActionIndex];
	const UInputAction*                                 InputAction   = ActionBinding->GetAction();

	const APlayerController* Controller = Char->GetController<APlayerController>();
	check(Controller)
	const ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();
	check(LocalPlayer)
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);
	check(Subsystem)

	Subsystem->InjectInputForAction(InputAction, 1.f, InputAction->Modifiers, InputAction->Triggers);

	return true;
}

bool FP14CheckItemsAmountLatentCommand::Update()
{
	const TArray<AP14InventoryItem*> ItemsLeft = P14::Test::GetAllActors<AP14InventoryItem>(World);
	return ItemsLeft.Num() == ItemsAmount;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Custom class of the latent command without macro. *
 * * * * * * * * * * * * * * * * * * * * * * * * * * */

bool P14::Test::FP14MoveLatentCommand::Update()
{
	if (!Char || !Char->InputComponent)
	{
		return true;
	}

	if (!InputAction)
	{
		const UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(Char->InputComponent);
		check(EnhancedInputComp)
		const int32 MoveActionIndex = P14::Test::GetActionBindingIndex(EnhancedInputComp, "IA_Move", ETriggerEvent::Triggered);
		check(MoveActionIndex != INDEX_NONE);
		const TUniquePtr<FEnhancedInputActionEventBinding>& ActionBinding = EnhancedInputComp->GetActionEventBindings()[MoveActionIndex];

		InputAction = ActionBinding->GetAction();
		check(InputAction)
	}
	if (!EnhancedInput)
	{
		const APlayerController* Controller = Char->GetController<APlayerController>();
		check(Controller)
		const ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();
		check(LocalPlayer)
		EnhancedInput = Cast<UEnhancedPlayerInput>(Controller->PlayerInput);
		check(EnhancedInput)
	}

	const double NewTime = FPlatformTime::Seconds();
	if (NewTime - StartTime < Time)
	{
		EnhancedInput->InjectInputForAction(InputAction, Direction, InputAction->Modifiers, InputAction->Triggers);
		return false;
	}

	return true;
}

bool P14::Test::FSimulateMovementLatentCommand::Update()
{
	if (!World || !Char)
	{
		return true;
	}

	if (!InputComp)
	{
		InputComp = Cast<UEnhancedInputComponent>(Char->InputComponent);
		check(InputComp)
	}
	if (!EnhancedInput)
	{
		const APlayerController* Controller = Char->GetController<APlayerController>();
		check(Controller)
		const ULocalPlayer* LocalPlayer = Controller->GetLocalPlayer();
		check(LocalPlayer)
		EnhancedInput = Cast<UEnhancedPlayerInput>(Controller->PlayerInput);
		check(EnhancedInput)
	}

	const TArray<TUniquePtr<FEnhancedInputActionEventBinding>>& ActionBindings = InputComp->GetActionEventBindings();
	while (FPlatformTime::Seconds() - StartTime >= Bindings[FrameIndex].WorldTime)
	{
		for (int32 Index = 0; Index < ActionBindings.Num(); ++Index)
		{
			const FVector Value = {Bindings[FrameIndex].ActionValues[Index].Value};
			if (Value.IsZero())
			{
				continue;
			}
			const UInputAction* InputAction = ActionBindings[Index]->GetAction();
			EnhancedInput->InjectInputForAction(InputAction, {Value}, InputAction->Modifiers, InputAction->Triggers);
		}
		if (++FrameIndex >= Bindings.Num())
		{
			return true;
		}
	}

	return false;
}

/* * * * * * * * * * * * * * * * *
 * Run tests method definitions. *
 * * * * * * * * * * * * * * * * */

bool FP14GameplayInventoryItemCanBeTakenOnJump::RunTest(const FString& Parameters)
{
	AddInfo("Test if item can be taken on jump.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Test1"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	const FString            ItemName      = "/Script/Engine.Blueprint'/Game/Project/PP14/Items/BP_InventoryItem.BP_InventoryItem'";
	const FTransform         ItemTransform = FTransform{FVector{0.f, 0.f, 250.f}};
	const AP14InventoryItem* Item          = P14::Test::CreateBlueprint<AP14InventoryItem>(World, ItemName, ItemTransform);
	UTEST_NOT_NULL_EXPR(Item)
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(World, 0);
	UTEST_NOT_NULL_EXPR(Char)

	// clang-format off
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(FP14JumpLatentCommand(Char))
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([this, World]() -> void
	{
		const TArray<AP14InventoryItem*> ItemsLeft = P14::Test::GetAllActors<AP14InventoryItem>(World);
		TestEqualExpr(ItemsLeft.Num(), 0);
	}, 2.f))
	// clang-format on

	return true;
}

bool FP14GameplayInventoryItemCanNotBeTakenOnJumpIfTooHigh::RunTest(const FString& Parameters)
{
	AddInfo("Test if item can be taken on jump.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Test1"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	const FString            ItemName      = "/Script/Engine.Blueprint'/Game/Project/PP14/Items/BP_InventoryItem.BP_InventoryItem'";
	const FTransform         ItemTransform = FTransform{FVector{0.f, 0.f, 500.f}};
	const AP14InventoryItem* Item          = P14::Test::CreateBlueprint<AP14InventoryItem>(World, ItemName, ItemTransform);
	UTEST_NOT_NULL_EXPR(Item)
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(World, 0);
	UTEST_NOT_NULL_EXPR(Char)

	// clang-format off
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(FP14JumpLatentCommand(Char))
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand([this, World]() -> void
	{
		const TArray<AP14InventoryItem*> ItemsLeft = P14::Test::GetAllActors<AP14InventoryItem>(World);
		TestEqualExpr(ItemsLeft.Num(), 1);
	}, 2.f))
	// clang-format on

	return true;
}

bool FP14GameplayAllItemsCanBeTakenOnMovement::RunTest(const FString& Parameters)
{
	AddInfo("Test if all items can be taken on movement.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Test1"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	constexpr float Distance  = 300.f;
	TArray<FVector> Locations = {
		{Distance * 1.f, 0.f, 100.f},
		{Distance * 2.f, 0.f, 100.f},
		{Distance * 3.f, 0.f, 100.f},
		{Distance * 3.f, 0.f, Distance},
		{Distance * 3.f, Distance * 1.f, 100.f},
		{Distance * 3.f, Distance * 2.f, 100.f}
	};

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(2.f));
	const FString ItemName = "/Script/Engine.Blueprint'/Game/Project/PP14/Items/BP_InventoryItem.BP_InventoryItem'";
	for (const FVector& Location : Locations)
	{
		const FTransform   Transform = {FRotator{0.f, 180.f, 0.f}, Location};
		AP14InventoryItem* Item      = P14::Test::CreateBlueprintDeferred<AP14InventoryItem>(World, ItemName, Transform);
		UTEST_NOT_NULL_EXPR(Item)
		Item->FinishSpawning(Transform);
	}
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(World, 0);
	UTEST_NOT_NULL_EXPR(Char)

	// Move forward towards first 3 items.
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(FP14CheckItemsAmountLatentCommand(World, Locations.Num()))
	ADD_LATENT_AUTOMATION_COMMAND(P14::Test::FP14MoveLatentCommand(Char, 1.5f, {0.f, 1.f}))
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f))
	ADD_LATENT_AUTOMATION_COMMAND(FP14CheckItemsAmountLatentCommand(World, Locations.Num() - 3))
	// Jump to the upper item.
	ADD_LATENT_AUTOMATION_COMMAND(FP14JumpLatentCommand(Char))
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(FP14CheckItemsAmountLatentCommand(World, Locations.Num() - 4))
	// Move right towards the last items.
	ADD_LATENT_AUTOMATION_COMMAND(P14::Test::FP14MoveLatentCommand(Char, 1.5f, {1.f, 0.f}))
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(0.5f))
	ADD_LATENT_AUTOMATION_COMMAND(FP14CheckItemsAmountLatentCommand(World, 0))

	return true;
}

bool FP14GameplayAllItemsCanBeTakenOnRecordedMovement::RunTest(const FString& Parameters)
{
	AddInfo("Test if all items can be taken on recorded movement.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_Root.P14_Root"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)
	ACharacter* Char = UGameplayStatics::GetPlayerCharacter(World, 0);
	UTEST_NOT_NULL_EXPR(Char)
	APlayerController* Controller = Char->GetController<APlayerController>();
	UTEST_NOT_NULL_EXPR(Controller)
	UTEST_TRUE_EXPR(P14::Test::GetAllActors<AP14InventoryItem>(World).Num() == 5);

	const FString Filename = P14::Test::GetTestDataFullPath();
	FP14InputData InputData;
	UTEST_TRUE_EXPR(P14::Test::FJsonUtils::ReadInputData(Filename, InputData));
	UTEST_TRUE_EXPR(!InputData.Bindings.IsEmpty())

	Char->SetActorTransform(InputData.InitialTransform);
	Controller->SetControlRotation(InputData.InitialTransform.Rotator());

	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f))
	ADD_LATENT_AUTOMATION_COMMAND(P14::Test::FSimulateMovementLatentCommand(World, Char, MoveTemp(InputData.Bindings)))
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand(1.f))
	TFunction<bool()> Lambda = [this, World]() -> bool
	{
		TestTrueExpr(P14::Test::GetAllActors<AP14InventoryItem>(World).IsEmpty());
		return true;
	};
	ADD_LATENT_AUTOMATION_COMMAND(FFunctionLatentCommand(MoveTemp(Lambda)))

	return true;
}

void FP14GameplayAllMapsShouldLoads::GetTests(TArray<FString>& OutBeautifiedNames, TArray<FString>& OutTestCommands) const
{
	const TArray<TTuple<FString, FString>> TestData =
	{
		{"MainMap", "/Game/Project/PP14/Level/P14_Root"},
		{"EmptyMap", "/Game/Project/PP14/Level/P14_TestEmpty"},
		{"TestMap", "/Game/Project/PP14/Level/P14_Test1"}
	};

	for (const TTuple<FString, FString>& OneTestData : TestData)
	{
		OutBeautifiedNames.Add(OneTestData.Key);
		OutTestCommands.Add(OneTestData.Value);
	}
}

bool FP14GameplayAllMapsShouldLoads::RunTest(const FString& Parameters)
{
	P14::Test::FLevelScope LevelScope{Parameters};
	ADD_LATENT_AUTOMATION_COMMAND(FEngineWaitLatentCommand{2.f});

	return true;
}

#endif
