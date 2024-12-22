// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P7PickupInterface.generated.h"

class AP7Item;
// This class does not need to be modified.
UINTERFACE()
class UP7PickupInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT07_API IP7PickupInterface
{
	GENERATED_BODY()

public:
	virtual void SetOverlappingItem(AP7Item* Item) = 0;
	virtual void AddCoins(const int32 Coins) = 0;
	virtual void AddSouls(const int32 Souls) = 0;
};
