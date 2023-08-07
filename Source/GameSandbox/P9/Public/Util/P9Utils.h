// Copyright (C) 2023, IKinder
// ReSharper disable CppUE4CodingStandardNamingViolationWarning

#pragma once

#include "P9Utils.generated.h"

UENUM(BlueprintType)
enum class EP9LogLevel : uint8
{
	TRACE   UMETA(DisplayName = "Trace"),
	DEBUG   UMETA(DisplayName = "Debug"),
	INFO    UMETA(DisplayName = "Info"),
	WARNING UMETA(DisplayName = "Warning"),
	ERROR   UMETA(DisplayName = "Error")	
};

UENUM(BlueprintType)
enum class EP9LogOutput : uint8
{
	ALL    UMETA(DisplayName = "All levels"),
	LOG    UMETA(DisplayName = "Output Log"),
	SCREEN UMETA(DisplayName = "Print Screen")
};

UENUM(BlueprintType)
enum class EP9AttackType : uint8
{
	MELEE_FIST UMETA(DisplayName = "Melee - Fist")
};

UENUM(BlueprintType)
enum class EP9CharState : uint8
{
	IDLE      UMETA(DisplayName = "Idle"),
	ATTACKING UMETA(DisplayName = "Attaching")
};
