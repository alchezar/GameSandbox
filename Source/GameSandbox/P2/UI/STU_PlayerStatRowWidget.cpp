// Copyright (C) 2023, IKinder

#include "STU_PlayerStatRowWidget.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"

void USTU_PlayerStatRowWidget::SetPlayerName(const FText& Text)
{
	if (!PlayerNameText) return;
	PlayerNameText->SetText(Text);
}

void USTU_PlayerStatRowWidget::SetKills(const FText& Text)
{
	if (!KillsText) return;
	KillsText->SetText(Text);
}

void USTU_PlayerStatRowWidget::SetDeaths(const FText& Text)
{
	if (!DeathsText) return;
	DeathsText->SetText(Text);
}

void USTU_PlayerStatRowWidget::SetTeams(const FText& Text)
{
	if (!TeamText) return;
	TeamText->SetText(Text);
}

void USTU_PlayerStatRowWidget::SetPlayerIndicatorVisibility(const bool Visible)
{
	if (!PlayerIndicatorImage) return;
	PlayerIndicatorImage->SetVisibility(Visible ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
}

void USTU_PlayerStatRowWidget::SetTeamColor(const FLinearColor& Color)
{
	if (!TeamImage) return;
	TeamImage->SetColorAndOpacity(Color);
}
