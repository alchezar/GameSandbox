// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12/Public/Inventory/P12InventoryItem.h"
#include "P12Medkit.generated.h"

UCLASS()
class GAMESANDBOX_API UP12Medkit : public UP12InventoryItem
{
	GENERATED_BODY()

public:
	virtual bool Consume(AP12BaseCharacter* ConsumeTarget) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	float Health = 25.f;
};
