// Copyright (C) 2023, IKinder

#include "UI/P10MissionCompleteWidget.h"

#include "Components/TextBlock.h"

void UP10MissionCompleteWidget::SetMissionText(const FString& NewText) const
{
	if (!MissionText) return;

	MissionText->SetText(FText::FromString(NewText));
}
