// Copyright (C) 2023, IKinder

#include "Widget/P7HealthBarWidget.h"

#include "Components/ProgressBar.h"

bool UP7HealthBarWidget::Initialize()
{
	return Super::Initialize();
}

void UP7HealthBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP7HealthBarWidget::SetHealthPercent(const float Percent) const
{
	if (!HealthBar) return;
	HealthBar->SetPercent(Percent);
}
