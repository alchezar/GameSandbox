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

	if (TryCacheInventoryComponent())
	{
		// ShowAllWeapons();
		// ShowAllAmmo();
	}
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
		const auto [WeaponID, DynamicInfo] = InventoryComponentCached->GetWeaponSlot(Index);
		UTexture2D* WeaponIcon = GameInstance->GetWeaponInfoByID(WeaponID)->WeaponIcon;
		SlotWeaponWidget->InitSlot(Index, WeaponIcon);
		SlotWeaponWidget->AddToViewport();
		
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
		InventoryComponentCached->OnAmmoChanged.AddUObject(SlotAmmoWidget, &UP13SlotAmmoWidget::OnAmmoChangedHandle);
		
		AmmoBox->AddChild(SlotAmmoWidget);
	}
}

bool UP13InGameWidget::TryCacheInventoryComponent()
{
	// AP13TopDownCharacter* Char = GetOwningPlayerPawn<AP13TopDownCharacter>();
	// AP13Weapon* CurrentWeapon = Char->GetCachedWeapon();
	
	const APawn* PlayerPawn = GetOwningPlayerPawn();
	if (!PlayerPawn)
	{
		return false;
	}
	InventoryComponentCached = PlayerPawn->FindComponentByClass<UP13InventoryComponent>();

	/* As NativeConstruct of the widgets fires before the BeginPlay of the Inventory,
	 * we will wait for the Inventory to create our widgets with updated information. */
	InventoryComponentCached->OnInventoryUpdated.AddUObject(this, &ThisClass::ShowStatWidgets);
	return true;
}

void UP13InGameWidget::ShowStatWidgets()
{
	ShowAllWeapons();
	ShowAllAmmo();
}
