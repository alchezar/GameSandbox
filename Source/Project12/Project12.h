// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"

namespace P12
{
	inline constexpr ECollisionChannel ECC_Climbing = ECC_GameTraceChannel6;
	inline constexpr ECollisionChannel ECC_Bullet   = ECC_GameTraceChannel7;
	inline constexpr ECollisionChannel ECC_Melee    = ECC_GameTraceChannel8;
}

#define ECC_CLIMBING ECC_GameTraceChannel6
