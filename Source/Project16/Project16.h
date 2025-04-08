// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"

namespace P16
{
	constexpr int32 CustomDepthRed = 250;

	namespace ZOrder
	{
		constexpr int32 MainOverlay = 0;
	}

	inline constexpr ECollisionChannel Gecc_Projectile = ECC_GameTraceChannel3;

	namespace Tag
	{
		inline const FName Enemy  = "Enemy";
		inline const FName Player = "Player";
	}

	namespace Rich
	{
		// Gaps.
		const FString Single = "\n";
		const FString Double = "\n\n";
		const FString Space  = " ";
		// Styles.
		const FString Cooldown = "<Cooldown>";
		const FString Damage   = "<Damage>";
		const FString Default  = "<Default>";
		const FString Level    = "<Level>";
		const FString Mana     = "<Mana>";
		const FString Percent  = "<Percent>";
		const FString Small    = "<Small>";
		const FString Time     = "<Time>";
		const FString Title    = "<Title>";
		// End of each style.
		const FString End = "</>";

		// Rich string wrapper.
		class FRichString
		{
		public:
			FRichString& Add(const FString& StyleName, const FString& Text);
			FRichString& Num(const FString& StyleName, const int32 Number);
			FRichString& Num(const FString& StyleName, const float Number);
			FRichString& Gap(const FString& GapName);
			FString      Get();

		private:
			FString String = "";
		};
	}
}

#define EARLY_RETURN_IF(Condition)              \
if (Condition) _UNLIKELY                        \
{                                               \
	return;                                     \
}

#define EARLY_RETURN_VALUE_IF(Condition, Value) \
if (Condition) _UNLIKELY                        \
{                                               \
	return Value;                               \
}

#define CONTINUE_IF(Condition)                  \
if (Condition)                                  \
{                                               \
	continue;                                   \
}
