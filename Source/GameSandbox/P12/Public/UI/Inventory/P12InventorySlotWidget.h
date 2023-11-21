// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P12InventorySlotWidget.generated.h"

class UImage;
struct FP12InventorySlot;

UCLASS()
class GAMESANDBOX_API UP12InventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void InitializeSlot(FP12InventorySlot& InventorySlot);
	void UpdateView();

protected:
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* ItemIcon;

private:
	FP12InventorySlot* LinkedSlot;
};
