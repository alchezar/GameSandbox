// Copyright © 2025, Ivan Kinder

#pragma once

#include "P17Types.generated.h"

class UP17LinkedAnimLayer;

USTRUCT(BlueprintType)
struct FP17WeaponData
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UP17LinkedAnimLayer> AnimLayer = nullptr;
};
