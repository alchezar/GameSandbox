// Copyright (C) 2023, IKinder

#pragma once

#include "P12CoreTypes.generated.h"

const FName SocketCharacterWeapon = "GripPoint";
const FName MontageSectionName_ReloadEnd = "ReloadEnd";

USTRUCT(BlueprintType)
struct FP12LedgeDescription
{
	GENERATED_BODY()

	FP12LedgeDescription()
		: Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator), Normal(FVector::ZeroVector) {}
	FP12LedgeDescription(const FVector& NewLocation, const FRotator& NewRotation, const FVector& NewNormal)
		: Location(NewLocation), Rotation(NewRotation), Normal(NewNormal) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Normal;
};

UENUM(BlueprintType)
enum class EP12EquipableItemType : uint8
{
	None = 0,
	Pistol,
	Rifle,
	Throwable,
	Melee,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EP12AmmunitionType : uint8
{
	None = 0,
	Pistol,
	Rifle,
	Shotgun,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EP12EquipmentSlot : uint8
{
	None = 0,
	SideArm,
	PrimaryWeapon,
	SecondaryWeapon,
	ThrowableSlot,
	MeleeWeapon,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EP12ReticleType : uint8
{
	None = 0,
	Default,
	Sniper,
	MAX UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EP12MeleeAttackType : uint8
{
	None = 0,
	Primary,
	Secondary,
	MAX UMETA(Hidden)
};
