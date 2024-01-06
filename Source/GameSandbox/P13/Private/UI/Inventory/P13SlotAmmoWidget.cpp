// Copyright © 2024, IKinder

#include "P13/Public/UI/Inventory/P13SlotAmmoWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "P13/Public/Game/P13GameInstance.h"
#include "P13/Public/Library/P13Types.h"

void UP13SlotAmmoWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP13SlotAmmoWidget::InitSlot(const FP13AmmoSlot NewAmmoSlot)
{
	TryCacheGameInstance();

	if (!AmmoIcons.Contains(NewAmmoSlot.WeaponType))
	{
		return;
	}

	WeaponType = NewAmmoSlot.WeaponType;
	AmmoCount = NewAmmoSlot.Count;
	MaxAmmoCount = NewAmmoSlot.MaxCount;
	AmmoImage->SetBrushFromTexture(AmmoIcons[NewAmmoSlot.WeaponType]);
	UpdateAmmoCount(NewAmmoSlot.Count);
	UpdateAmmoUsageStatus(EP13AmmoType::Default);
}

void UP13SlotAmmoWidget::OnWeaponChangedHandle(const FP13WeaponSlot& NewWeaponSlot, int32 WeaponIndex)
{
	UpdateAmmoUsageStatus(NewWeaponSlot.AmmoType);
}

void UP13SlotAmmoWidget::OnAmmoChangedHandle(const EP13AmmoType CurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount)
{
	if (WeaponType != CurrentWeaponType || InInventoryNewCount < 0)
	{
		return;
	}
	AmmoCount = InInventoryNewCount;
	UpdateAmmoCount(InInventoryNewCount);
}

void UP13SlotAmmoWidget::TryCacheGameInstance()
{
	if (GameInstanceCached.IsValid())
	{
		return;
	}
	GameInstanceCached = GetWorld()->GetGameInstance<UP13GameInstance>();
	check(GameInstanceCached)
}

void UP13SlotAmmoWidget::UpdateAmmoCount(const int32 NewCount) const
{
	AmmoCountText->SetText(FText::AsNumber(NewCount));
	AmmoCountText->SetColorAndOpacity(NewCount == MaxAmmoCount ? FLinearColor::Green : FLinearColor::White);
}

void UP13SlotAmmoWidget::UpdateAmmoUsageStatus(const EP13AmmoType TypeToCompare) const
{
	const bool bActive = WeaponType == TypeToCompare;
	BackgroundBorder->SetBrushColor(bActive ? ActiveBorderColor : InactiveBorderColor);
}
