// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P14InputRecordingTypes.generated.h"

USTRUCT()
struct FP14ActionData
{
	GENERATED_BODY()

	UPROPERTY()
	FName Name = "";

	UPROPERTY()
	FVector Value = {};
};

USTRUCT()
struct FP14BindingsData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FP14ActionData> ActionValues = {};

	UPROPERTY()
	float WorldTime = 0.f;
};

USTRUCT()
struct FP14InputData
{
	GENERATED_BODY()

	UPROPERTY()
	TArray<FP14BindingsData> Bindings = {};

	UPROPERTY()
	FTransform InitialTransform = {};
};
