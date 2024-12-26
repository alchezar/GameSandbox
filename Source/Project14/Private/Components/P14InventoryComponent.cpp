// Copyright © 2024, Ivan Kinder

#include "Components/P14InventoryComponent.h"

UP14InventoryComponent::UP14InventoryComponent()
{}

void UP14InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	for (int32 Index = 0; Index < StaticCast<uint8>(EP14InventoryItemType::MAX); ++Index)
	{
		const EP14InventoryItemType ItemType    = StaticCast<EP14InventoryItemType>(Index);
		const bool                  bLimitCheck = InventoryLimits.Contains(ItemType);
		checkf(bLimitCheck, TEXT("Limit for %s doesn't exist"), ItemType)
	}
}

bool UP14InventoryComponent::TryAddItem(const FP14InventoryData& InData)
{
	if (InData.Score <= 0 || !InventoryLimits.Contains(InData.Type))
	{
		return false;
	}

	if (!Inventory.Contains(InData.Type))
	{
		Inventory.Add(InData.Type, 0);
	}

	const int32 NextScore = Inventory[InData.Type] + InData.Score;
	if (NextScore > InventoryLimits[InData.Type])
	{
		return false;
	}

	Inventory[InData.Type] = FMath::Min(NextScore, InventoryLimits[InData.Type]);
	OnScoreUpdated.Broadcast(InData.Type);
	return true;
}

int32 UP14InventoryComponent::GetInventoryAmountByType(const EP14InventoryItemType InType) const
{
	return Inventory.Contains(InType) ? Inventory[InType] : 0;
}
