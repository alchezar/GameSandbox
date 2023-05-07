// Copyright (C) 2023, IKinder

#include "ER_GameHud.h"

#include "ER_GameModeBase.h"
#include "Components/TextBlock.h"

void UER_GameHud::InitializeHud(AER_GameModeBase* GameMode)
{
	if (!GameMode) return;

	CoinsCount->SetText(FText::AsNumber(0));
	GameMode->OnCoinsCountChanged.AddUObject(this, &ThisClass::SetCoinsCount);
}

void UER_GameHud::SetCoinsCount(int32 NewCount)
{
	CoinsCount->SetText(FText::AsNumber(NewCount));
}
