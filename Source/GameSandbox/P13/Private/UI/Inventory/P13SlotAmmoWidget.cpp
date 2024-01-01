// Copyright © 2024, IKinder

#include "P13/Public/UI/Inventory/P13SlotAmmoWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "P13/Public/Library/P13Types.h"

void UP13SlotAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP13SlotAmmoWidget::InitSlot(const FP13AmmoSlot NewAmmoSlot)
{
	if (!AmmoIcons.Contains(NewAmmoSlot.WeaponType))
	{
		return;
	}

	WeaponType = NewAmmoSlot.WeaponType;
	MaxCount = NewAmmoSlot.MaxCount;
	UpdateAmmoCount(NewAmmoSlot.Count);
	AmmoImage->SetBrushFromTexture(AmmoIcons[NewAmmoSlot.WeaponType]);
}

void UP13SlotAmmoWidget::UpdateAmmoCount(const int32 NewCount) const
{
	AmmoCount->SetText(FText::AsNumber(NewCount));
	AmmoCount->SetColorAndOpacity(NewCount == MaxCount ? FLinearColor::Green : FLinearColor::White);
}

void UP13SlotAmmoWidget::OnAmmoChangedHandle(const EP13WeaponType CurrentWeaponType, const int32 NewCount) const
{
	if (WeaponType != CurrentWeaponType)
	{
		return;
	}
	UpdateAmmoCount(NewCount);
}
