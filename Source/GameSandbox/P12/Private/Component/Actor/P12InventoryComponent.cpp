// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Actor/P12InventoryComponent.h"

#include "P12/Public/Inventory/P12InventoryItem.h"
#include "P12/Public/UI/Inventory/P12InventoryViewWidget.h"

#pragma region FP12InventorySlot

void FP12InventorySlot::BindOnInventorySlotUpdate(const FP12OnInventorySlotUpdateSignature& Callback) const
{
	OnInventorySlotUpdate = Callback;
}

void FP12InventorySlot::UnbindOnInventorySlotUpdate(const UObject* Object)
{
	if (!OnInventorySlotUpdate.IsBound())
	{
		return;
	}
	OnInventorySlotUpdate.RemoveAll(Object);
}

void FP12InventorySlot::UpdateSlotState()
{
	OnInventorySlotUpdate.Broadcast();
}

void FP12InventorySlot::ClearSlot()
{
	Item = nullptr;
	Count = 0;
	UpdateSlotState();
}

#pragma endregion FP12InventorySlot

#pragma region UP12InventoryComponent

void UP12InventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	InventorySlots.AddDefaulted(Capacity);
}

void UP12InventoryComponent::OpenViewInventory(APlayerController* InPlayerController)
{
	if (!InventoryViewWidget)
	{
		CreateViewWidget(InPlayerController);
	}
	if (!InventoryViewWidget->IsVisible())
	{
		InventoryViewWidget->AddToViewport();
	}
}

void UP12InventoryComponent::CloseViewInventory()
{
	if (InventoryViewWidget->IsVisible())
	{
		InventoryViewWidget->RemoveFromParent();
	}
}

bool UP12InventoryComponent::GetIsViewVisible() const
{
	return InventoryViewWidget && InventoryViewWidget->IsVisible();
}

bool UP12InventoryComponent::TryAddItem(TWeakObjectPtr<UP12InventoryItem> ItemToAdd, int32 Count)
{
	if (!ItemToAdd.IsValid() || Count < 0)
	{
		return false;
	}
	FP12InventorySlot* FreeSlot = FindFreeSlot();
	if (!FreeSlot)
	{
		return false;
	}
	FreeSlot->Item = ItemToAdd;
	FreeSlot->Count = Count;
	++ItemsInInventory;
	FreeSlot->UpdateSlotState();
	
	return true;
}

bool UP12InventoryComponent::TryRemoveItem(const FName& ItemID)
{
	const FP12InventorySlot* ItemSlot = FindItemSlot(ItemID);
	if (!ItemSlot)
	{
		return false;
	}
	InventorySlots.RemoveAll([=](const FP12InventorySlot& Slot){ return Slot.Item->GetDataTableID() == ItemID; });
	return true;
}

TArray<FText> UP12InventoryComponent::GetAllItemsNames() const
{
	TArray<FText> Result;
	for (const FP12InventorySlot& Slot : InventorySlots)
	{
		if (!Slot.Item.IsValid())
		{
			continue;
		}
		Result.Add(Slot.Item->GetDescription().Name);
	}
	return Result;
}

void UP12InventoryComponent::CreateViewWidget(APlayerController* InPlayerController)
{
	if (!InPlayerController || !InventoryViewWidgetClass || InventoryViewWidget)
	{
		return;
	}
	InventoryViewWidget = CreateWidget<UP12InventoryViewWidget>(InPlayerController, InventoryViewWidgetClass);
	InventoryViewWidget->InitializeViewWidget(&InventorySlots);
}

FP12InventorySlot* UP12InventoryComponent::FindItemSlot(FName ItemID)
{
	return InventorySlots.FindByPredicate([=](const FP12InventorySlot& Slot){ return Slot.Item->GetDataTableID() == ItemID; });
}

FP12InventorySlot* UP12InventoryComponent::FindFreeSlot()
{
	return InventorySlots.FindByPredicate([=](const FP12InventorySlot& Slot){ return !Slot.Item.IsValid(); });

}

#pragma endregion UP12InventoryComponent
