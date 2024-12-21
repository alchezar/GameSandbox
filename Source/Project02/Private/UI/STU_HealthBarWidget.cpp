// Copyright (C) 2023, IKinder

#include "UI/STU_HealthBarWidget.h"

#include "Components/ProgressBar.h"

void USTU_HealthBarWidget::SetHealthPercent(const float Percent) const
{
	if (!HealthProgressBar) return;

	const ESlateVisibility HeathBarVisibility = (Percent > VisibilityThreshold || FMath::IsNearlyZero(Percent))
		                                            ? ESlateVisibility::Hidden
		                                            : ESlateVisibility::Visible;
	HealthProgressBar->SetVisibility(HeathBarVisibility);
	const FLinearColor HealthBarColor = Percent > ColorThreshold ? GoodColor : BadColor;
	HealthProgressBar->SetFillColorAndOpacity(HealthBarColor);
	HealthProgressBar->SetPercent(Percent);
}
