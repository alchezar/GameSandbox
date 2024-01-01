// Copyright © 2024, IKinder

#include "P13/Public/UI/Inventory/P13SlotWeaponWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"

void UP13SlotWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP13SlotWeaponWidget::InitSlot(const int32 NewWeaponIndex, UTexture2D* NewWeaponImage)
{
	WeaponIndex->SetText(FText::AsNumber(NewWeaponIndex));
	WeaponImage->SetBrushFromTexture(NewWeaponImage);
}
