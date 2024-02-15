// Copyright © 2024, IKinder

#include "P13/Public/UI/Inventory/P13SlotWeaponWidget.h"

#include "Components/Border.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "P13/Public/Game/P13GameInstance.h"

void UP13SlotWeaponWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP13SlotWeaponWidget::InitSlot(const int32 NewWeaponIndex, const FP13WeaponSlot& NewWeaponSlot)
{
	TryCacheGameInstance();

	CurrentWeaponIndex = NewWeaponIndex;
	WeaponIndexText->SetText(FText::AsNumber(NewWeaponIndex + 1));
	WeaponImage->SetBrushFromTexture(GameInstanceCached->GetWeaponInfoByID(NewWeaponSlot.WeaponID)->WeaponIcon);

	AmmoType = NewWeaponSlot.AmmoType;
	MagazineCapacity = NewWeaponSlot.MaxRound;
	MagazineCurrent = NewWeaponSlot.DynamicInfo.Round;

	/* The index of the first active weapon will be 0, so compare with it. */
	UpdateWeaponUsageStatus();
	UpdateAmmoCount(MagazineCurrent);
}

void UP13SlotWeaponWidget::OnCurrentWeaponUpdatedHandle(const FP13WeaponSlot& NewWeaponSlot, const int32 WeaponIndex)
{
	UpdateWeaponUsageStatus(WeaponIndex);

	if (IsAnimationPlaying(ReloadAnim))
	{
		SetAnimationCurrentTime(ReloadAnim, 1.f - LastPercentage);
		PauseAnimation(ReloadAnim);
	}
}

void UP13SlotWeaponWidget::OnAmmoChangedHandle(const EP13AmmoType CurrentWeaponType, const int32 InWeaponNewCount, const int32 InInventoryNewCount)
{
	if (AmmoType != CurrentWeaponType || InWeaponNewCount < 0)
	{
		return;
	}
	UpdateAmmoCount(InWeaponNewCount);
}

void UP13SlotWeaponWidget::OnWeaponStartReloadingHandle(UAnimMontage* CharFireMontage, const int32 WeaponIndex, const float ReloadingTime)
{
	const float PlaybackSpeed = 1.f / FMath::Max(ReloadingTime, 0.1f);
	PlayAnimation(ReloadAnim, 0.f, 1, EUMGSequencePlayMode::Forward, PlaybackSpeed);
}

void UP13SlotWeaponWidget::OnWeaponFinishReloadingHandle(const int32 NewRoundNum, const int32 WeaponIndex, const bool bSuccess)
{
	ReloadingProgressBar->SetPercent(1.f);
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

	WeaponIndexText->SetColorAndOpacity(bActive ? FColor::Green : FColor::White);
	BackgroundBorder->SetBrushColor(bActive ? ActiveBorderColor : InactiveBorderColor);

	AmmoCountText->SetVisibility(bActive ? ESlateVisibility::Visible : ESlateVisibility::Collapsed);
}

void UP13SlotWeaponWidget::UpdateAmmoCount(const int32 NewCount)
{
	AmmoCountText->SetText(FText::AsNumber(NewCount));
	AmmoCountText->SetColorAndOpacity(NewCount == MagazineCapacity ? FLinearColor::Green : FLinearColor::White);

	LastPercentage = 1.f - NewCount / StaticCast<float>(MagazineCapacity);
	ReloadingProgressBar->SetPercent(LastPercentage);
}
