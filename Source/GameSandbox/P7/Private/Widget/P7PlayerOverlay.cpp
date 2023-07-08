// Copyright (C) 2023, IKinder

#include "P7/Public/Widget/P7PlayerOverlay.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

bool UP7PlayerOverlay::Initialize()
{
	return Super::Initialize();
}

void UP7PlayerOverlay::NativeConstruct()
{
	Super::NativeConstruct();
	SetHealthPercent(1.f);
	SetStaminaPercent(1.f);
	AddCoins(0);
	AddSouls(0);
}

void UP7PlayerOverlay::SetHealthPercent(const float Percent)
{
	HealthProgressBar->SetPercent(Percent);
}

void UP7PlayerOverlay::SetStaminaPercent(const float Percent)
{
	StaminaProgressBar->SetPercent(Percent);
}

void UP7PlayerOverlay::AddCoins(const int32 NewCoins)
{
	Coins += NewCoins;
	CoinCount->SetText(FText::AsNumber(Coins));
}

void UP7PlayerOverlay::AddSouls(const int32 NewSouls)
{
	Souls += NewSouls;
	SoulCount->SetText(FText::AsNumber(Souls));
}
