// Copyright © 2024, IKinder

#include "P13/Public/UI/P13InGameWidget.h"

#include "Components/HorizontalBox.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/UI/Inventory/P13SlotAmmoWidget.h"
#include "P13/Public/UI/Inventory/P13SlotWeaponWidget.h"

void UP13InGameWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CacheInventoryComponent();
}

void UP13InGameWidget::ShowAllWeapons() const
{
	check(InventoryComponentCached.IsValid())
	const UP13GameInstance* GameInstance = GetWorld()->GetGameInstance<UP13GameInstance>();
	check(GameInstance)

	WeaponBox->ClearChildren();

	for (int32 Index = 0; Index < InventoryComponentCached->GetWeaponSlotsCount(); ++Index)
	{
		UP13SlotWeaponWidget* SlotWeaponWidget = CreateWidget<UP13SlotWeaponWidget>(GetOwningPlayer(), WeaponSlotClass);
		if (!SlotWeaponWidget)
		{
			continue;
		}
		SlotWeaponWidget->InitSlot(Index, InventoryComponentCached->GetWeaponSlot(Index).WeaponID);
		SlotWeaponWidget->AddToViewport();
		InventoryComponentCached->OnSwitchWeapon.AddUObject(SlotWeaponWidget, &UP13SlotWeaponWidget::OnWeaponChangedHandle);
		InventoryComponentCached->OnAmmoChanged.AddUObject(SlotWeaponWidget, &UP13SlotWeaponWidget::OnAmmoChangedHandle);
		
		WeaponBox->AddChild(SlotWeaponWidget);
	}
}

void UP13InGameWidget::ShowAllAmmo() const
{
	check(InventoryComponentCached.IsValid())

	AmmoBox->ClearChildren();

	for (int32 Index = 0; Index < InventoryComponentCached->GetAmmoSlotsCount(); ++Index)
	{
		UP13SlotAmmoWidget* SlotAmmoWidget = CreateWidget<UP13SlotAmmoWidget>(GetOwningPlayer(), AmmoSlotClass);
		if (!SlotAmmoWidget)
		{
			continue;
		}
		SlotAmmoWidget->InitSlot(InventoryComponentCached->GetAmmoSlot(Index));
		SlotAmmoWidget->AddToViewport();
		InventoryComponentCached->OnSwitchWeapon.AddUObject(SlotAmmoWidget, &UP13SlotAmmoWidget::OnWeaponChangedHandle);
		InventoryComponentCached->OnAmmoChanged.AddUObject(SlotAmmoWidget, &UP13SlotAmmoWidget::OnAmmoChangedHandle);
		
		AmmoBox->AddChild(SlotAmmoWidget);
	}
}

void UP13InGameWidget::CacheInventoryComponent()
{
	const APawn* PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn)
	{
		return;
	}
	InventoryComponentCached = PlayerPawn->FindComponentByClass<UP13InventoryComponent>();

	/* As NativeConstruct of the widgets fires before the BeginPlay of the Inventory,
	 * we will wait for the Inventory to create our widgets with updated information. */
	InventoryComponentCached->OnInventoryUpdated.AddUObject(this, &ThisClass::ShowStatWidgets);
}

void UP13InGameWidget::ShowStatWidgets()
{
	ShowAllWeapons();
	ShowAllAmmo();
}
