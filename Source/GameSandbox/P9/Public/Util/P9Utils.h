// Copyright (C) 2023, IKinder

// ReSharper disable CppUE4CodingStandardNamingViolationWarning

#pragma once
#include "Engine/DataTable.h"
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
	ATTACKING UMETA(DisplayName = "Attaching"),
	PUNCHED   UMETA(DisplayName = "Punched")
};

USTRUCT(BlueprintType)
struct FP9MeleeCollisionProfile
{
	GENERATED_BODY()

	FP9MeleeCollisionProfile()
		: Enabled(FName("Weapon")), Disabled(FName("NoCollision")) {}

	FP9MeleeCollisionProfile(const FString& EnabledStr, const FString& DisabledStr)
		: Enabled(FName(EnabledStr)), Disabled(FName(DisabledStr)) {}

	UPROPERTY(EditAnywhere, Category = "C++ | Components")
	FName Enabled;
	UPROPERTY(EditAnywhere, Category = "C++ | Components")
	FName Disabled;
};

USTRUCT(BlueprintType)
struct FP9PlayerAttackMontage : public FTableRowBase
{
	GENERATED_BODY()

	FP9PlayerAttackMontage()
		: Montage(nullptr), AnimSectionCount(0), Description("Empty") {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UAnimMontage* Montage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AnimSectionCount;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;
};
