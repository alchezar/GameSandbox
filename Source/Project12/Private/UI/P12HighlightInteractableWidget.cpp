// Copyright (C) 2023, IKinder

#include "UI/P12HighlightInteractableWidget.h"

#include "Components/TextBlock.h"

void UP12HighlightInteractableWidget::SetActionText(FName KeyName)
{
	if (!ActionText)
	{
		return;
	}
	ActionText->SetText(FText::FromName(KeyName));
}
