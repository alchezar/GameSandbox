// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Data/P16LootTiersDataAsset.h"

#include "Project16.h"

TArray<FP16LootItem> UP16LootTiersDataAsset::GetLootItems() const
{
	TArray<FP16LootItem> Result;

	for (const FP16LootItemProbable& Probable : LootItems)
	{
		for (int32 Index = 0; Index < Probable.MaxNumberToSpawn; ++Index)
		{
			CONTINUE_IF(FMath::RandRange(0.f, 1.f) > Probable.ChanceToSpawn)
			Result.Add(Probable.Item);
		}
	}

	return Result;
}
