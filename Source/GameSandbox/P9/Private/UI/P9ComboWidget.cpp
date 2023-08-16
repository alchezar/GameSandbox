// Copyright (C) 2023, IKinder

#include "P9/Public/UI/P9ComboWidget.h"

#include "Components/TextBlock.h"

void UP9ComboWidget::NativeConstruct()
{
	Super::NativeConstruct();
	check(TextCombo)
}

void UP9ComboWidget::UpdateCompoCount(const int32 Value)
{
	if (Value < 1) return;
	if (TextCombo->GetVisibility() != ESlateVisibility::Visible)
	{
		TextCombo->SetVisibility(ESlateVisibility::Visible);
	}
	TextCombo->SetText(FText::FromString(FString::FromInt(Value) + "x Combo"));

	if (AnimComboFade && AnimComboShake)
	{
		PlayAnimation(AnimComboFade);
		PlayAnimation(AnimComboShake);
	}
}

void UP9ComboWidget::ResetCombo() const
{
	TextCombo->SetVisibility(ESlateVisibility::Collapsed);
}
