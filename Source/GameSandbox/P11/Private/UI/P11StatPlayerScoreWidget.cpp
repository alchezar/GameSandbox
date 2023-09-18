// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11StatPlayerScoreWidget.h"

#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Internationalization/Text.h"

void UP11StatPlayerScoreWidget::NativePreConstruct()
{
	Super::NativePreConstruct();

	PlayerNameText->SetText(PlayerName);
	DeathText->SetText(FText::AsNumber(Deaths));
	KillsText->SetText(FText::AsNumber(Kills));
	PingText->SetText(FText::AsNumber(Ping));
	DeadImg->SetVisibility(bDead ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
}

void UP11StatPlayerScoreWidget::SetPlayerName(const FText& NewName)
{
	PlayerName = NewName;
	PlayerNameText->SetText(PlayerName);
}

void UP11StatPlayerScoreWidget::SetIsDead(const bool bNewDead)
{
	bDead = bNewDead;
	DeadImg->SetColorAndOpacity(bNewDead ? FLinearColor::White : FLinearColor(1.f, 1.f, 1.f, 0.f));
}

void UP11StatPlayerScoreWidget::SetKills(const int32 NewKills)
{
	Kills = NewKills;
	KillsText->SetText(FText::AsNumber(Kills));
}

void UP11StatPlayerScoreWidget::SetDeaths(const int32 NewDeaths)
{
	Deaths = NewDeaths;
	DeathText->SetText(FText::AsNumber(Deaths));
}

void UP11StatPlayerScoreWidget::SetPing(const float NewPing)
{
	Ping = NewPing;
	PingText->SetText(FText::AsNumber(Ping));
}
