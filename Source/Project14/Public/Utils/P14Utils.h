// Copyright © 2024, Ivan Kinder

#pragma once
#include "EngineUtils.h"
#include "EnhancedInputComponent.h"
#include "Misc/OutputDeviceNull.h"
#include "Tests/AutomationCommon.h"
// ReSharper disable CppUnusedIncludeDirective
#include "Utils/P14JsonUtils.h"
// ReSharper restore CppUnusedIncludeDirective

class UEnhancedInputLocalPlayerSubsystem;

namespace P14::Test
{
#define TestEqualExpr(Actual, Expected) TestEqual(TEXT(#Actual), Actual, Expected)
#define TestNullExpr(Actual) TestNull(TEXT(#Actual), Actual)

	inline constexpr EAutomationTestFlags TestContext = EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ProgramContext;

	template <typename T1, typename T2>
	struct TTestPayload
	{
		T1    Test;
		T2    Expected;
		float Tolerance = KINDA_SMALL_NUMBER;
	};

	template <typename T, typename L, typename... Args>
	void IterateThroughEnum(const T Max, const L& Lambda, Args... Arguments)
	{
		for (int32 Index = 0; Index < StaticCast<uint8>(Max); ++Index)
		{
			Lambda(StaticCast<T>(Index), Arguments...);
		}
	}

	FORCEINLINE _NODISCARD FString GetTestDataFullPath()
	{
		return FPaths::Combine(FPaths::ProjectDir(), "Data", "Project14", "CharacterInputData.json");
	}

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

	FORCEINLINE _NODISCARD int32 GetActionBindingIndex(const UEnhancedInputComponent* InInputComp, const FString InActionName, const ETriggerEvent InTriggerEvent)
	{
		int32 Result = INDEX_NONE;
		if (!InInputComp)
		{
			return Result;
		}

		for (int32 Index = 0; Index < InInputComp->GetActionEventBindings().Num(); ++Index)
		{
			const UInputAction* InputAction  = InInputComp->GetActionEventBindings()[Index]->GetAction();
			const ETriggerEvent TriggerEvent = InInputComp->GetActionEventBindings()[Index]->GetTriggerEvent();

			if (InputAction->GetName() == InActionName && TriggerEvent == InTriggerEvent)
			{
				Result = Index;
				break;
			}
		}

		return Result;
	}

	FORCEINLINE void CallFuncByNameWithParams(UObject* InObject, const FString& InFuncName, const TArray<FString>& InParams)
	{
		if (!InObject)
		{
			return;
		}

		FString Command = InFuncName;
		for (const FString& Param : InParams)
		{
			Command.Append(" ").Append(Param);
		}

		FOutputDeviceNull OutputDeviceNull{};
		InObject->CallFunctionByNameWithArguments(*Command, OutputDeviceNull, nullptr, true);
	}

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
