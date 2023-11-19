// Copyright (C) 2023, IKinder

#include "P12/Public/UI/P12AttributeProgressBarWidget.h"

#include "Components/ProgressBar.h"
#include "P12/Public/Player/P12BaseCharacter.h"

void UP12AttributeProgressBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	CachedCharacter = GetOwningPlayerPawn<AP12BaseCharacter>();
	if (!CachedCharacter.IsValid())
	{
		return;
	}
	CachedCharacter->OnHealthChange.AddUObject(this, &ThisClass::OnHealthChangedHandle);
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
