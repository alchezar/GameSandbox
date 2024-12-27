// Copyright © 2024, Ivan Kinder

#pragma once

namespace P14
{
	inline constexpr EAutomationTestFlags TestContext = EAutomationTestFlags::EditorContext | EAutomationTestFlags::CommandletContext | EAutomationTestFlags::ProgramContext;

	template <typename T1, typename T2>
	struct TTestPayload
	{
		T1    Test;
		T2    Expected;
		float Tolerance = KINDA_SMALL_NUMBER;
	};

	template <typename T>
	concept HasMax = requires(T Enum) { T::MAX; };

	template <HasMax T, typename L, typename... Args>
	void IterateThroughEnum(const L& Lambda, Args... Arguments)
	{
		for (int32 Index = 0; Index < StaticCast<uint8>(T::MAX); ++Index)
		{
			Lambda(StaticCast<T>(Index), Arguments...);
		}
	}
}
