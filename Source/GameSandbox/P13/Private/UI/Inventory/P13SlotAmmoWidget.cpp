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
	MaxAmmoCount = NewAmmoSlot.MaxCount;
	UpdateAmmoCount(NewAmmoSlot.Count);
	AmmoImage->SetBrushFromTexture(AmmoIcons[NewAmmoSlot.WeaponType]);
}

void UP13SlotAmmoWidget::OnWeaponChangedHandle(FName WeaponID, const FP13WeaponDynamicInfo* DynamicInfo, int32 WeaponIndex)
{
	const FP13WeaponInfo* WeaponInfo = GameInstanceCached->GetWeaponInfoByID(WeaponID);
	if (!WeaponInfo)
	{
		return;
	}

	UpdateAmmoUsageStatus(WeaponInfo->AmmoType);
}

void UP13SlotAmmoWidget::OnAmmoChangedHandle(const EP13WeaponType CurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount) const
{
	if (WeaponType != CurrentWeaponType)
	{
		return;
	}
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

void UP13SlotAmmoWidget::UpdateAmmoUsageStatus(const EP13WeaponType TypeToCompare) const
{
	const bool bActive = WeaponType == TypeToCompare;
	BackgroundBorder->SetBrushColor(bActive ? ActiveBorderColor : InactiveBorderColor);
}
