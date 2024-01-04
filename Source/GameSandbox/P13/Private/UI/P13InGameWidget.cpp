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
	
	GameInstanceCached = GetWorld()->GetGameInstance<UP13GameInstance>();
	check(GameInstanceCached.IsValid())

	CacheInventoryComponent();
}

void UP13InGameWidget::ShowAllWeapons() const
{
	check(InventoryComponentCached.IsValid())

	WeaponBox->ClearChildren();

	for (int32 Index = 0; Index < InventoryComponentCached->GetWeaponSlotsCount(); ++Index)
	{
		AddNewWeaponSlotWidget(Index, InventoryComponentCached->GetWeaponSlot(Index).WeaponID);
	}
}

void UP13InGameWidget::ShowAllAmmo() const
{
	check(InventoryComponentCached.IsValid())

	AmmoBox->ClearChildren();

	for (int32 Index = 0; Index < InventoryComponentCached->GetAmmoSlotsCount(); ++Index)
	{
		AddNewAmmoSlotWidget(InventoryComponentCached->GetAmmoSlot(Index));
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
	InventoryComponentCached->OnNewWeaponTaken.AddUObject(this, &ThisClass::OnNewWeaponTakenHandle);
	InventoryComponentCached->OnNewAmmoTaken.AddUObject(this, &ThisClass::OnNewAmmoTakenHandle);
}

void UP13InGameWidget::ShowStatWidgets()
{
	ShowAllWeapons();
	ShowAllAmmo();
}

void UP13InGameWidget::OnNewWeaponTakenHandle(const int32 NewWeaponIndex, const FName NewWeaponID)
{
	AddNewWeaponSlotWidget(NewWeaponIndex, NewWeaponID);
}

void UP13InGameWidget::OnNewAmmoTakenHandle(FP13AmmoSlot NewAmmoSlot)
{
	AddNewAmmoSlotWidget(NewAmmoSlot);
}

void UP13InGameWidget::AddNewWeaponSlotWidget(const int32 NewWeaponIndex, const FName NewWeaponID) const
{
	UP13SlotWeaponWidget* SlotWeaponWidget = CreateWidget<UP13SlotWeaponWidget>(GetOwningPlayer(), WeaponSlotClass);
	if (!SlotWeaponWidget)
	{
		return;
	}
	SlotWeaponWidget->InitSlot(NewWeaponIndex, NewWeaponID);
	SlotWeaponWidget->AddToViewport();
	InventoryComponentCached->OnSwitchWeapon.AddUObject(SlotWeaponWidget, &UP13SlotWeaponWidget::OnWeaponChangedHandle);
	InventoryComponentCached->OnAmmoChanged.AddUObject(SlotWeaponWidget, &UP13SlotWeaponWidget::OnAmmoChangedHandle);

	WeaponBox->AddChild(SlotWeaponWidget);
}

void UP13InGameWidget::AddNewAmmoSlotWidget(const FP13AmmoSlot& NewAmmoSlot) const
{
	UP13SlotAmmoWidget* SlotAmmoWidget = CreateWidget<UP13SlotAmmoWidget>(GetOwningPlayer(), AmmoSlotClass);
	if (!SlotAmmoWidget)
	{
		return;
	}
	SlotAmmoWidget->InitSlot(NewAmmoSlot);
	SlotAmmoWidget->AddToViewport();
	InventoryComponentCached->OnSwitchWeapon.AddUObject(SlotAmmoWidget, &UP13SlotAmmoWidget::OnWeaponChangedHandle);
	InventoryComponentCached->OnAmmoChanged.AddUObject(SlotAmmoWidget, &UP13SlotAmmoWidget::OnAmmoChangedHandle);

	AmmoBox->AddChild(SlotAmmoWidget);
}
