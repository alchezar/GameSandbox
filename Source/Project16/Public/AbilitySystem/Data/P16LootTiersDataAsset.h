// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Util/P16Type.h"
#include "P16LootTiersDataAsset.generated.h"

UCLASS()
class PROJECT16_API UP16LootTiersDataAsset : public UDataAsset
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	TArray<FP16LootItem> GetLootItems() const;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++ | Loot")
	TArray<FP16LootItemProbable> LootItems;
};
