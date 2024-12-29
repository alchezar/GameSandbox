// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "P14Types.generated.h"

UENUM(BlueprintType)
enum class EP14InventoryItemType : uint8
{
	Sphere = 0,
	Cube,
	Cylinder,
	MAX UMETA(Hidden)
};

USTRUCT(BlueprintType)
struct FP14InventoryData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EP14InventoryItemType Type;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0"))
	int32 Score;

	FString ToString() const
	{
		return FString::Printf(TEXT("(Type=%i,Score=%i)"), Type, Score);
	}

	bool operator==(const FP14InventoryData& Other) const
	{
		return Type == Other.Type && Score == Other.Score;
	}
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FP14OnScoreUpdated, EP14InventoryItemType, Type);
