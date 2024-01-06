// Copyright © 2024, IKinder

#include "P13/Public/UI/Inventory/P13InventoryStatsWidget.h"

#include "Components/HorizontalBox.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/UI/Inventory/P13SlotAmmoWidget.h"
#include "P13/Public/UI/Inventory/P13SlotWeaponWidget.h"

void UP13InventoryStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	GameInstanceCached = GetWorld()->GetGameInstance<UP13GameInstance>();
	check(GameInstanceCached.IsValid())

	CacheInventoryComponent();
}

void UP13InventoryStatsWidget::ShowAllWeapons() const
{
	check(InventoryComponentCached.IsValid())

	WeaponBox->ClearChildren();

	for (int32 Index = 0; Index < InventoryComponentCached->GetWeaponSlotsCount(); ++Index)
	{
		AddNewWeaponSlotWidget(Index, InventoryComponentCached->GetWeaponSlot(Index));
	}
}

void UP13InventoryStatsWidget::ShowAllAmmo() const
{
	check(InventoryComponentCached.IsValid())

	AmmoBox->ClearChildren();

	InventoryComponentCached->SortAmmoSlots();
	for (int32 Index = 0; Index < InventoryComponentCached->GetAmmoSlotsCount(); ++Index)
	{
		AddNewAmmoSlotWidget(InventoryComponentCached->GetAmmoSlot(Index));
	}
}

void UP13InventoryStatsWidget::OnNewWeaponTakenHandle(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot)
{
	AddNewWeaponSlotWidget(NewWeaponIndex, NewWeaponSlot);
	ShowAllAmmo();
}

void UP13InventoryStatsWidget::OnNewAmmoTakenHandle(const FP13AmmoSlot& NewAmmoSlot)
{
	AddNewAmmoSlotWidget(NewAmmoSlot);
	ShowAllAmmo();
}

void UP13InventoryStatsWidget::CacheInventoryComponent()
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

void UP13InventoryStatsWidget::ShowStatWidgets()
{
	ShowAllWeapons();
	ShowAllAmmo();
}

void UP13InventoryStatsWidget::AddNewWeaponSlotWidget(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot) const
{
	UP13SlotWeaponWidget* SlotWeaponWidget = CreateWidget<UP13SlotWeaponWidget>(GetOwningPlayer(), WeaponSlotClass);
	if (!SlotWeaponWidget)
	{
		return;
	}
	SlotWeaponWidget->InitSlot(NewWeaponIndex, NewWeaponSlot);
	SlotWeaponWidget->AddToViewport();
	InventoryComponentCached->OnSwitchWeapon.AddUObject(SlotWeaponWidget, &UP13SlotWeaponWidget::OnWeaponChangedHandle);
	InventoryComponentCached->OnAmmoChanged.AddUObject(SlotWeaponWidget, &UP13SlotWeaponWidget::OnAmmoChangedHandle);

	WeaponBox->AddChild(SlotWeaponWidget);
}

void UP13InventoryStatsWidget::AddNewAmmoSlotWidget(const FP13AmmoSlot& NewAmmoSlot) const
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
