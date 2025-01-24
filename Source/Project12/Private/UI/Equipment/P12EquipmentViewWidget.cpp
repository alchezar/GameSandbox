// Copyright (C) 2023, IKinder

#include "UI/Equipment/P12EquipmentViewWidget.h"

#include "Component/Actor/P12EquipmentComponent.h"
#include "Components/VerticalBox.h"
#include "UI/Equipment/P12EquipmentSlotWidget.h"

void UP12EquipmentViewWidget::InitializeViewWidget(UP12EquipmentComponent* InEquipmentComponent)
{
	LinkedEquipmentComponent = InEquipmentComponent;
	TArray<AP12EquipableItem*> Items = LinkedEquipmentComponent->GetItems();
	/* Skip first "None" slot. */
	for (int32 Index = 1; Index < Items.Num(); ++Index)
	{
		AddEquipmentSlotView(Items[Index], Index);
	}
}

void UP12EquipmentViewWidget::AddEquipmentSlotView(AP12EquipableItem* LinkToWeapon, const int32 SlotIndex)
{
	check(DefaultSlotWidgetClass)
	UP12EquipmentSlotWidget* SlotWidget = CreateWidget<UP12EquipmentSlotWidget>(this, DefaultSlotWidgetClass);
	check(SlotWidget)

	SlotWidget->InitializeSlot(LinkToWeapon, SlotIndex);
	VerticalBoxWeaponSlots->AddChildToVerticalBox(SlotWidget);
	SlotWidget->UpdateView();
	SlotWidget->OnEquipmentDropInSlot.BindUObject(this, &ThisClass::EquipEquipmentToSlot);
	SlotWidget->OnEquipmentRemoveFromSlot.AddUObject(this, &ThisClass::RemoveEquipmentFromSlot);
}

void UP12EquipmentViewWidget::UpdateSlot(int32 SlotIndex)
{
	UP12EquipmentSlotWidget* WidgetToUpdate = Cast<UP12EquipmentSlotWidget>(VerticalBoxWeaponSlots->GetChildAt(SlotIndex - 1));
	if (!WidgetToUpdate)
	{
		return;
	}
	WidgetToUpdate->InitializeSlot(LinkedEquipmentComponent->GetItems()[SlotIndex], SlotIndex);
	WidgetToUpdate->UpdateView();
}

bool UP12EquipmentViewWidget::EquipEquipmentToSlot(const TSubclassOf<AP12EquipableItem>& WeaponClass, const int32 SenderIndex)
{
	if (LinkedEquipmentComponent->AddEquipmentItemToSlot(WeaponClass, SenderIndex))
	{
		UpdateSlot(SenderIndex);
		return true;
	}
	return false;
}

void UP12EquipmentViewWidget::RemoveEquipmentFromSlot(int32 SlotIndex)
{
	LinkedEquipmentComponent->RemoveItemFromSlot(SlotIndex);
}
