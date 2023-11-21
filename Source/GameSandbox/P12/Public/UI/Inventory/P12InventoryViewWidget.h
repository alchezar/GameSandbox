// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12InventoryViewWidget.generated.h"

struct FP12InventorySlot;

UCLASS()
class GAMESANDBOX_API UP12InventoryViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeViewWidget(TArray<FP12InventorySlot>* InventorySlots);
};
