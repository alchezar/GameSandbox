// Copyright (C) 2023, IKinder

#include "P12/Public/UI/P12PlayerHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "P12/Public/Player/P12BaseCharacter.h"

void UP12PlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	CachedCharacter = GetOwningPlayerPawn<AP12BaseCharacter>();
	if (!CachedCharacter.IsValid())
	{
		return;
	}
	CachedCharacter->OnHealthChange.AddUObject(this, &ThisClass::OnHealthChangeHandle);
	CachedCharacter->OnAimingStateChanged.AddUObject(this, &ThisClass::OnAimStateChangedHandle);
	CachedCharacter->OnAmmoCountChanged.AddUObject(this, &ThisClass::OnAmmoCountChangedHandle);
	OnHealthChangeHandle(CachedCharacter->GetHeathPercent(), 1.f);
}

void UP12PlayerHUDWidget::OnHealthChangeHandle(const float Health, const float MaxHealth)
{
	HealthBar->SetPercent(Health / MaxHealth);
}

void UP12PlayerHUDWidget::OnAimStateChangedHandle(const bool bStart)
{
	PlayAnimation(ReticleAiming, 0.f, 1, bStart ? EUMGSequencePlayMode::Forward : EUMGSequencePlayMode::Reverse);
}

void UP12PlayerHUDWidget::OnAmmoCountChangedHandle(const int32 Ammo)
{
	AmmoText->SetText(FText::AsNumber(Ammo));
}
