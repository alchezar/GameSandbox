// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"

namespace P16
{
	/// @brief Depth values for highlighting on hover in postprocess material.
	constexpr int32 CustomDepthRed = 250;

	/// @brief The number of points for the arcane shards spell.
	constexpr int32 PointsNum = 12;

	/// @brief Orders for correct widget layout.
	namespace ZOrder
	{
		constexpr int32 MainOverlay = 0;
	}

	///
	/// @brief Custom collision trace channels.
	///
	namespace CollisionChannel
	{
		inline constexpr ECollisionChannel Projectile = ECC_GameTraceChannel3;
		inline constexpr ECollisionChannel Weapon     = ECC_GameTraceChannel5;
		inline constexpr ECollisionChannel Cursor     = ECC_GameTraceChannel9;
	}

	///
	/// @brief Base tags, not gameplay ones.
	///
	namespace Tag
	{
		inline const FName Enemy  = "Enemy";
		inline const FName Player = "Player";
	}

	///
	/// @brief Socket names.
	///
	namespace Socket
	{
		inline const FName Weapon    = "TipSocket";
		inline const FName LeftHand  = "LeftHand";
		inline const FName RightHand = "RightHand";
	}

	///
	/// @brief Formatting rick text.
	///
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

///
/// @brief Custom macros.
///

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
