// Copyright (C) 2023, IKinder

#include "UI/P12AttributeProgressBarWidget.h"

#include "Components/ProgressBar.h"
#include "Player/P12BaseCharacter.h"

void UP12AttributeProgressBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP12AttributeProgressBarWidget::SetProgressPercentage(float Percentage)
{
	ProgressBar->SetPercent(Percentage);
}

void UP12AttributeProgressBarWidget::OnHealthChangedHandle(float Health, float MaxHealth)
{
	if (FMath::IsNearlyZero(MaxHealth))
	{
		ProgressBar->SetPercent(0.f);
		return;
	}
	ProgressBar->SetPercent(Health / MaxHealth);
}

void UP12AttributeProgressBarWidget::CacheOwner(AP12BaseCharacter* BaseChar)
{
	CachedCharacter = BaseChar;
	if (!CachedCharacter.IsValid())
	{
		return;
	}
	CachedCharacter->OnHealthChange.AddUObject(this, &ThisClass::OnHealthChangedHandle);
}
