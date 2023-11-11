// Copyright (C) 2023, IKinder

#include "P12/Public/UI/P12PlayerHUDWidget.h"

#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "P12/Public/Player/P12BaseCharacter.h"

void UP12PlayerHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// CachedCharacter = GetOwningPlayerPawn<AP12BaseCharacter>();
	// if (!CachedCharacter.IsValid())
	// {
	// 	return;
	// }
	// CachedCharacter->OnHealthChange.AddUObject(this, &ThisClass::OnHealthChangeHandle);
	// CachedCharacter->OnAmmoCountChanged.AddUObject(this, &ThisClass::OnAmmoCountChangedHandle);
	// OnHealthChangeHandle(CachedCharacter->GetHeathPercent(), 1.f);
}

void UP12PlayerHUDWidget::SetCachedCharacter(AP12BaseCharacter* NewCharacter)
{
	CachedCharacter = NewCharacter;
	if (!CachedCharacter.IsValid())
	{
		return;
	}
	CachedCharacter->OnHealthChange.AddUObject(this, &ThisClass::OnHealthChangeHandle);
	CachedCharacter->OnAmmoCountChanged.AddUObject(this, &ThisClass::OnAmmoCountChangedHandle);
	OnHealthChangeHandle(CachedCharacter->GetHeathPercent(), 1.f);
}

void UP12PlayerHUDWidget::OnHealthChangeHandle(const float Health, const float MaxHealth)
{
	HealthBar->SetPercent(Health / MaxHealth);
}

void UP12PlayerHUDWidget::OnAmmoCountChangedHandle(const int32 Ammo, const int32 TotalAmmo)
{
	AmmoText->SetText(FText::AsNumber(Ammo));
	TotalAmmoText->SetText(FText::AsNumber(TotalAmmo));
}
