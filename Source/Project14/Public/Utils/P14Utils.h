// Copyright © 2024, Ivan Kinder

#pragma once

#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "Tests/AutomationCommon.h"
#include "Utils/P14JsonUtils.h"

class UEnhancedInputLocalPlayerSubsystem;

namespace P14::Test
{
	// ReSharper disable CppUE4CodingStandardNamingViolationWarning
#define TestEqualExpr(Actual, Expected) TestEqual(TEXT(#Actual), Actual, Expected)
#define TestNullExpr(Actual) TestNull(TEXT(#Actual), Actual)
	// ReSharper restore CppUE4CodingStandardNamingViolationWarning

	/// Flags used for automation tests
	inline constexpr EAutomationTestFlags TestContext = EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ProgramContext;

	/// Safe getter for the test data folder path.
	/// @return Full path to test data folder
	_NODISCARD FString GetTestDataFullPath();

	/// Find the index of an action binding in the enhanced input component object that matches the provided action name and trigger event.
	/// @param InInputComp Enhanced input component object to search in
	/// @param InActionName Name of the action to search for
	/// @param InTriggerEvent Trigger event to search for
	/// @return Index of the action binding
	_NODISCARD int32 GetActionBindingIndex(const UEnhancedInputComponent* InInputComp, const FString& InActionName, const ETriggerEvent InTriggerEvent);

	/// Call a blueprint function with the provided parameters
	/// @param InObject Object to call the function on
	/// @param InFuncName Name of the function to call
	/// @param InParams Parameters to pass to the function
	void CallBlueprintFuncByNameWithParams(UObject* InObject, const FString& InFuncName, const TArray<FString>& InParams);

	/// Inject input for an action into the enhanced input component object
	/// @param InController Player controller to inject input for
	/// @param InActionName Name of the action to inject input for
	/// @param InActionValue Input value to inject
	void InjectActionInput(const APlayerController* InController, const FString& InActionName, FInputActionValue&& InActionValue);

	/// Getter for any available world.
	/// @details Refactored copy of the AutomationCommon::GetAnyGameWorld()
	/// @return Currently available world.
	_NODISCARD UWorld* GetTestGameWorld();

	/// Payload struct for automation tests
	/// @tparam T1 Type of the test value
	/// @tparam T2 Type of the expected value
	template <typename T1, typename T2>
	struct TTestPayload
	{
		T1    Test;
		T2    Expected;
		float Tolerance = KINDA_SMALL_NUMBER;
	};

	/// Iterate through an enum and call a lambda for each value
	/// @tparam T Enum type
	/// @tparam L Lambda type
	/// @tparam Args Lambda argument types
	/// @param Max Maximum value of the enum to iterate through
	/// @param Lambda Lambda to call for each value
	/// @param Arguments Arguments to pass to the lambda
	template <typename T, typename L, typename... Args>
	void IterateThroughEnum(const T Max, const L& Lambda, Args... Arguments)
	{
		for (int32 Index = 0; Index < StaticCast<uint8>(Max); ++Index)
		{
			Lambda(StaticCast<T>(Index), Arguments...);
		}
	}

	/// Create a blueprint actor and return a pointer to it
	/// @tparam T Actor type to create
	/// @param InWorld World to spawn the actor in
	/// @param InName Full path of the blueprint to spawn
	/// @param InTransform Transform to spawn the actor with
	/// @return Pointer to the spawned actor
	template <typename T>
	_NODISCARD T* CreateBlueprint(UWorld* InWorld, const FString& InName, const FTransform& InTransform = FTransform::Identity)
	{
		const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *InName);
		if (!InWorld || !Blueprint)
		{
			return nullptr;
		}

		return InWorld->SpawnActor<T>(Blueprint->GeneratedClass, InTransform);
	}

	/// Create a deferred blueprint actor and return a pointer to it
	/// @tparam T Actor type to create
	/// @param InWorld World to spawn the actor in
	/// @param InName Full path of the blueprint to spawn
	/// @param InTransform Transform to spawn the actor with
	/// @return Pointer to the spawned actor
	template <typename T>
	_NODISCARD T* CreateBlueprintDeferred(UWorld* InWorld, const FString& InName, const FTransform& InTransform = FTransform::Identity)
	{
		const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *InName);
		if (!InWorld || !Blueprint)
		{
			return nullptr;
		}

		return InWorld->SpawnActorDeferred<T>(Blueprint->GeneratedClass, InTransform);
	}

	/// Find a widget of a specific type in a world
	/// @tparam T Widget type to find
	/// @param WorldContextObject Object to get the world from
	/// @return Pointer to the found widget
	template <typename T>
	T* FindWidgetByClass(UObject* WorldContextObject)
	{
		TArray<UUserWidget*> Widgets;
		UWidgetBlueprintLibrary::GetAllWidgetsOfClass(WorldContextObject, Widgets, T::StaticClass(), false);

		return Widgets.IsEmpty() ? nullptr : Cast<T>(Widgets[0]);
	}

	/// Find a child widget with a specific name
	/// @tparam T Widget type to find
	/// @param ParentWidget Widget to search in
	/// @param ChildName Name of the child to find
	template <typename T>
	T* FindWidgetChildByName(const UUserWidget* ParentWidget, const FString& ChildName)
	{
		if (!ParentWidget || !ParentWidget->WidgetTree)
		{
			return nullptr;
		}

		UWidget* FoundWidget = nullptr;
		UWidgetTree::ForWidgetAndChildren(ParentWidget->WidgetTree->RootWidget, [ChildName, &FoundWidget](UWidget* Child) -> void
		{
			if (Child && Child->GetName() == ChildName)
			{
				FoundWidget = Child;
			}
		});

		return Cast<T>(FoundWidget);
	}

	/// Get all actors of a specific type in a world
	/// @tparam T Actor type to get
	/// @param InWorld World to get the actors from
	/// @return Array of pointers to the actors
	template <typename T>
	_NODISCARD TArray<T*> GetAllActors(UWorld* InWorld)
	{
		TArray<T*> Actors = {};
		for (TActorIterator<T> Iterator{InWorld}; Iterator; ++Iterator)
		{
			if (!*Iterator)
			{
				continue;
			}
			Actors.Add(*Iterator);
		}

		return Actors;
	}

	/// @class FLevelScope
	/// @brief Level scope class
	/// @details Level scope class to manage the scope of a level in a game, automatically opens and closes the level.
	class FLevelScope
	{
	public:
		explicit FLevelScope(const FString& InMapName)
		{
			AutomationOpenMap(InMapName);
		};

		~FLevelScope()
		{
			ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand)
		};
	};

	/// @class FP14MoveLatentCommand
	/// @brief Latent command to simulate movement
	/// @details Latent command for moving a character in the test world.
	class FP14MoveLatentCommand : public IAutomationLatentCommand
	{
	public:
		FP14MoveLatentCommand(ACharacter* InChar, const float InTime, const FVector2D InDirection)
			: Char(InChar)
			, Time(InTime)
			, Direction(InDirection)
		{}

		virtual bool Update() override;

	private:
		ACharacter*           Char          = nullptr;
		float                 Time          = 0.f;
		FVector2D             Direction     = {};
		UEnhancedPlayerInput* EnhancedInput = nullptr;
		const UInputAction*   InputAction   = nullptr;
	};

	/// @class FSimulateMovementLatentCommand
	/// @brief Latent command to simulate movement
	/// @details Latent command for moving a character in the test world by reading pre-recorded input bindings.
	class FSimulateMovementLatentCommand : public IAutomationLatentCommand
	{
	public:
		FSimulateMovementLatentCommand(UWorld* InWorld, ACharacter* InChar, TArray<FP14BindingsData> InBindings)
			: World(InWorld)
			, Char(InChar)
			, Bindings(MoveTemp(InBindings))
		{};

		virtual bool Update() override;

	private:
		TObjectPtr<UWorld>       World      = nullptr;
		TObjectPtr<ACharacter>   Char       = nullptr;
		TArray<FP14BindingsData> Bindings   = {};
		int32                    FrameIndex = 0;

		UEnhancedInputComponent* InputComp     = nullptr;
		UEnhancedPlayerInput*    EnhancedInput = nullptr;
	};
}
