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
	}}

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
