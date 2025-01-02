// Copyright © 2024, Ivan Kinder

#include "Tests/P14Character_Tests.h"

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Misc/AutomationTest.h"
#include "Player/P14Character.h"
#include "Tests/AutomationCommon.h"
#include "Tests/P14Utils.h"

#if WITH_AUTOMATION_TESTS

DEFINE_LOG_CATEGORY_STATIC(LogP14CharacterTest, All, All)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14CharacterHealthMightBeChangedWithDamage, "Project14.Character.HealthMightBeChangedWithDamage", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14LatentCommandSimpleWait, "Project14.LatentCommand.SimpleWait", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14LatentCommandSimpleLog, "Project14.LatentCommand.SimpleLog", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14LatentCommandOpenCloseMap, "Project14.LatentCommand.OpenCloseMap", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14CharacterCanBeKilled, "Project14.Character.CanBeKilled", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FP14CharacterAutoHealWorks, "Project14.Character.AutoHealWorks", P14::Test::TestContext
	| EAutomationTestFlags::ProductFilter
	| EAutomationTestFlags::HighPriority)

bool FP14CharacterHealthMightBeChangedWithDamage::RunTest(const FString& Parameters)
{
	// Open level and get the world.
	AddInfo("Test if character health might be changed with damage.");
	P14::Test::FLevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld* World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	// Spawn deferred character.
	const FString    BPCharName = "/Script/Engine.Blueprint'/Game/Project/PP14/Tests/BP_Character_P14_Test.BP_Character_P14_Test'";
	const FTransform InitTransforms{FVector{0.f, -240.f, 110.f}};
	AP14Character*   Char = P14::Test::CreateBlueprintDeferred<AP14Character>(World, BPCharName, InitTransforms);
	UTEST_NOT_NULL_EXPR(Char)

	// Set health data.
	constexpr FP14HealthData HealthData = {1000.f, 10.f, 0.5f, 5.f};
	P14::Test::CallFuncByNameWithParams(Char, "SetHealthData", {HealthData.ToString()});
	Char->FinishSpawning(InitTransforms);

	// Test if character health might be changed with damage.
	TestTrueExpr(FMath::IsNearlyEqual(Char->GetHealthPercent(), 1.f));
	constexpr float DamageAmount = 10.f;
	Char->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
	TestEqualExpr(Char->GetHealthPercent(), 1.f - DamageAmount / HealthData.MaxHealth);

	return true;
}

bool FP14LatentCommandSimpleWait::RunTest(const FString& Parameters)
{
	AddInfo("Test if latent command simple wait works.");

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand{3.f});

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_ONE_PARAMETER(FP14LatentLogCommand, FString, LogMessage);

bool FP14LatentLogCommand::Update()
{
	UE_LOG(LogP14CharacterTest, Display, TEXT("%s"), *LogMessage);
	return true;
}

bool FP14LatentCommandSimpleLog::RunTest(const FString& Parameters)
{
	AddInfo("Test if latent command simple log works.");

	UE_LOG(LogP14CharacterTest, Warning, TEXT("Log test 1"));
	ADD_LATENT_AUTOMATION_COMMAND(FP14LatentLogCommand{ "Latent log test 1" })
	UE_LOG(LogP14CharacterTest, Warning, TEXT("Log test 2"));
	ADD_LATENT_AUTOMATION_COMMAND(FP14LatentLogCommand{ "Latent log test 2" })
	UE_LOG(LogP14CharacterTest, Warning, TEXT("Log test 3"));

	return true;
}

bool FP14LatentCommandOpenCloseMap::RunTest(const FString& Parameters)
{
	AddInfo("Test if latent command open/close map works.");

	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};

	return true;
}

DEFINE_LATENT_AUTOMATION_COMMAND_TWO_PARAMETER(FP14CharacterDestroyedLatentCommand, ACharacter*, Char, float, LifeSpan);

bool FP14CharacterDestroyedLatentCommand::Update()
{
	const double NewTime = FPlatformTime::Seconds();
	if (NewTime - StartTime >= LifeSpan)
	{
		if (IsValid(Char))
		{
			UE_LOG(LogP14CharacterTest, Error, TEXT("Character %s is not destroyed."), *Char->GetName());
		}
		return true;
	}
	return false;
}

bool FP14CharacterCanBeKilled::RunTest(const FString& Parameters)
{
	// Open level and get the world.
	AddInfo("Test if character can be killed.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	// Spawn deferred character.
	const FString    BPCharName = "/Script/Engine.Blueprint'/Game/Project/PP14/Tests/BP_Character_P14_Test.BP_Character_P14_Test'";
	const FTransform InitTransforms{FVector{0.f, -240.f, 110.f}};
	AP14Character*   Char = P14::Test::CreateBlueprintDeferred<AP14Character>(World, BPCharName, InitTransforms);
	UTEST_NOT_NULL_EXPR(Char)

	// Set health data.
	constexpr FP14HealthData HealthData = {1000.f, 10.f, 0.5f, 1.5f};
	P14::Test::CallFuncByNameWithParams(Char, "SetHealthData", {HealthData.ToString()});
	Char->FinishSpawning(InitTransforms);

	// Test if character can be killed.
	TestTrueExpr(FMath::IsNearlyEqual(Char->GetHealthPercent(), 1.f));
	constexpr float KillingDamageAmount = HealthData.MaxHealth;
	Char->TakeDamage(KillingDamageAmount, FDamageEvent{}, nullptr, nullptr);
	TestEqualExpr(Char->GetHealthPercent(), 0.f);
	TestEqualExpr(Char->GetCharacterMovement()->MovementMode, MOVE_None);
	TestEqualExpr(Char->GetCapsuleComponent()->GetCollisionEnabled(), ECollisionEnabled::NoCollision);
	TestTrueExpr(Char->GetMesh()->IsSimulatingPhysics());
	TestTrueExpr(Char->GetMesh()->GetCollisionProfileName() == "Ragdoll");
	TestNullExpr(Char->GetController());
	TestEqualExpr(Char->GetLifeSpan(), HealthData.LifeSpan);

	ADD_LATENT_AUTOMATION_COMMAND(FP14CharacterDestroyedLatentCommand(Char, HealthData.LifeSpan * 2.f));

	return true;
}

bool FP14CharacterAutoHealWorks::RunTest(const FString& Parameters)
{
	// Open level and get the world.
	AddInfo("Test if character auto heal works.");
	P14::Test::FLevelScope LevelScope{"/Game/Project/PP14/Level/P14_TestEmpty"};
	UWorld*                World = AutomationCommon::GetAnyGameWorld();
	UTEST_NOT_NULL_EXPR(World)

	// Spawn deferred character.
	const FString    BPCharName = "/Script/Engine.Blueprint'/Game/Project/PP14/Tests/BP_Character_P14_Test.BP_Character_P14_Test'";
	const FTransform InitTransforms{FVector{0.f, -240.f, 110.f}};
	AP14Character*   Char = P14::Test::CreateBlueprintDeferred<AP14Character>(World, BPCharName, InitTransforms);
	UTEST_NOT_NULL_EXPR(Char)

	// Set health data.
	constexpr FP14HealthData HealthData = {1000.f, 100.f, 0.5f, 2.0f};
	P14::Test::CallFuncByNameWithParams(Char, "SetHealthData", {HealthData.ToString()});
	Char->FinishSpawning(InitTransforms);

	TestTrueExpr(FMath::IsNearlyEqual(Char->GetHealthPercent(), 1.f));
	constexpr float DamageAmount = HealthData.MaxHealth / 2.f;
	Char->TakeDamage(DamageAmount, FDamageEvent{}, nullptr, nullptr);
	const float HealthDiff   = HealthData.MaxHealth * (1.f - Char->GetHealthPercent());
	const float HealDuration = HealthData.HealRate * HealthDiff / HealthData.HealModifier;

	TFunction<void()> Lambda = [Char]() -> void
	{
		const float CurrentHealth = Char->GetHealthPercent();
		if (Char->GetHealthPercent() < 1.f)
		{
			UE_LOG(LogP14CharacterTest, Error, TEXT("Character %s is not healed, current health is %f."), *Char->GetName(), CurrentHealth);
		}
	};

	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(MoveTemp(Lambda), HealDuration + 1.f));

	return true;
}

#endif
