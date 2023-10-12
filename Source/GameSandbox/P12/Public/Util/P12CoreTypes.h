// Copyright (C) 2023, IKinder

#pragma once

#include "P12CoreTypes.generated.h"

USTRUCT(BlueprintType)
struct FP12LedgeDescription
{
	GENERATED_BODY()

	FP12LedgeDescription()
		: Location(FVector::ZeroVector), Rotation(FRotator::ZeroRotator) {}
	FP12LedgeDescription(const FVector& NewLocation, const FRotator& NewRotation)
		: Location(NewLocation), Rotation(NewRotation) {}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector Location;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator Rotation;
};