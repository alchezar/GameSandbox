// Copyright © 2024, Ivan Kinder

#pragma once
#include "Tests/AutomationCommon.h"

namespace P14::Test
{
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

	template <typename T>
	T* CreateBlueprint(UWorld* InWorld, const FString& InName, const FTransform& InTransform = FTransform::Identity)
	{
		const UBlueprint* Blueprint = LoadObject<UBlueprint>(nullptr, *InName);
		if (!InWorld || !Blueprint)
		{
			return nullptr;
		}

		return InWorld->SpawnActor<T>(Blueprint->GeneratedClass, InTransform);
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
}
