// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12InventoryViewWidget.generated.h"

class UP12InventorySlotWidget;
class UGridPanel;
struct FP12InventorySlot;

UCLASS()
class PROJECT12_API UP12InventoryViewWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeViewWidget(TArray<FP12InventorySlot>& InventorySlots);

protected:
	void AddItemSlotView(FP12InventorySlot& SlotToAdd);

protected:
	UPROPERTY(meta = (BindWidget))
	UGridPanel* GridPanelItemsSlot;

	UPROPERTY(EditDefaultsOnly, Category = "C++")
	TSubclassOf<UP12InventorySlotWidget> InventorySlotWidgetClass;
	UPROPERTY(EditDefaultsOnly, Category = "C++")
	int32 ColumnCount = 4;
};
