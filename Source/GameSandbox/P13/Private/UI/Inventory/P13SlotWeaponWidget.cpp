// Copyright © 2024, IKinder

#include "P13/Public/UI/Inventory/P13SlotWeaponWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "P13/Public/Game/P13GameInstance.h"

void UP13SlotWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP13SlotWeaponWidget::InitSlot(const int32 NewWeaponIndex, const FName NewWeaponID)
{
	TryCacheGameInstance();
	
	CurrentWeaponIndex = NewWeaponIndex;
	WeaponIndexText->SetText(FText::AsNumber(NewWeaponIndex + 1));
	WeaponImage->SetBrushFromTexture(GameInstanceCached->GetWeaponInfoByID(NewWeaponID)->WeaponIcon);

	if (const FP13WeaponInfo* WeaponInfo = GameInstanceCached->GetWeaponInfoByID(NewWeaponID))
	{
		WeaponType = WeaponInfo->AmmoType;
		MagazineCapacity = WeaponInfo->MaxRound;
	}

	/* The index of the first active weapon will be 0, so compare with it. */
	UpdateWeaponUsageStatus();
	UpdateAmmoCount(MagazineCapacity);
}

void UP13SlotWeaponWidget::OnWeaponChangedHandle(FName WeaponID, const FP13WeaponDynamicInfo* DynamicInfo, int32 WeaponIndex)
{
	UpdateWeaponUsageStatus(WeaponIndex);
}

void UP13SlotWeaponWidget::OnAmmoChangedHandle(const EP13WeaponType CurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount) const
{
	if (WeaponType != CurrentWeaponType)
	{
		return;
	}
	UpdateAmmoCount(InWeaponNewCount);
}

void UP13SlotWeaponWidget::TryCacheGameInstance()
{
	if (GameInstanceCached.IsValid())
	{
		return;
	}
	GameInstanceCached = GetWorld()->GetGameInstance<UP13GameInstance>();
	check(GameInstanceCached)
}

void UP13SlotWeaponWidget::UpdateWeaponUsageStatus(const int32 IndexToCompare) const
{
	const bool bActive = CurrentWeaponIndex == IndexToCompare;

	WeaponIndexText->SetColorAndOpacity(bActive ? FColor::Green : FColor::Red);
	BackgroundBorder->SetBrushColor(bActive ? ActiveBorderColor : InactiveBorderColor);
	
	AmmoCountText->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UP13SlotWeaponWidget::UpdateAmmoCount(const int32 NewCount) const
{
	AmmoCountText->SetText(FText::AsNumber(NewCount));
	AmmoCountText->SetColorAndOpacity(NewCount == MagazineCapacity ? FLinearColor::Green : FLinearColor::White);
}
