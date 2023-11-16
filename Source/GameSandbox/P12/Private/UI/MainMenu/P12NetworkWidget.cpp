// Copyright (C) 2023, IKinder

#include "P12/Public/UI/MainMenu/P12NetworkWidget.h"

FText UP12NetworkWidget::GetNetworkType() const
{
	return bLAN ? FText::FromString("LAN") : FText::FromString("Internet");
}

void UP12NetworkWidget::ToggleNetworkType()
{
	bLAN = !bLAN;
}

void UP12NetworkWidget::CloseWidget()
{
	OnNetworkWidgetClose.Broadcast();
	// SetVisibility(ESlateVisibility::Collapsed);
}
