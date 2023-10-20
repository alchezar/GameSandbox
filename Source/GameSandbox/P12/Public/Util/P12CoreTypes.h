// Copyright (C) 2023, IKinder

#pragma once

#include "P12CoreTypes.generated.h"

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
enum class EP12EquipablItemType : uint8
{
	None,
	Pistol
};