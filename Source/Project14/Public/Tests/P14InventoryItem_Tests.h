// Copyright © 2024, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Items/P14InventoryItem.h"
#include "P14InventoryItem_Tests.generated.h"

namespace P14::Test
{
	_NODISCARD FORCEINLINE UWorld* GetTestGameWorld()
	{
		for (const FWorldContext& Context : GEngine->GetWorldContexts())
		{
			if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World())
			{
				return Context.World();
			}
		}
		return nullptr;
	}
}

UCLASS(Placeable)
class PROJECT14_API AP14InventoryItem_Test : public AP14InventoryItem
{
	GENERATED_BODY()

public:
	_NODISCARD float GetRadius() const { return Radius; }
};
