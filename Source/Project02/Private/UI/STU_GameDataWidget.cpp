// Copyright (C) 2023, IKinder

#include "UI/STU_GameDataWidget.h"

#include "Game/STU_GameModeBase.h"
#include "Player/STU_PlayerState.h"

int32 USTU_GameDataWidget::GetKillsNum() const
{
	const ASTU_PlayerState* PlayerState = GetPlayerState();
	return PlayerState ? PlayerState->GetKillsNum() : 0;
}

int32 USTU_GameDataWidget::GetDeathsNum() const
{
	const ASTU_PlayerState* PlayerState = GetPlayerState();
	return PlayerState ? PlayerState->GetDeathNum() : 0;
}

int32 USTU_GameDataWidget::GetCurrentRoundNum() const
{
	const ASTU_GameModeBase* GameMode = GetGameMode();
	return GameMode ? GameMode->GetCurrentRound() : 0;
}

int32 USTU_GameDataWidget::GetTotalRoundNum() const
{
	const ASTU_GameModeBase* GameMode = GetGameMode();
	return GameMode ? GameMode->GetGameData().RoundsNum : 0;
}

int32 USTU_GameDataWidget::GetRoundSecondsRemaining() const
{
	const ASTU_GameModeBase* GameMode = GetGameMode();
	return GameMode ? GameMode->GetRoundCountDown() : 0;
}

ASTU_GameModeBase* USTU_GameDataWidget::GetGameMode() const
{
	return GetWorld() ? Cast<ASTU_GameModeBase>(GetWorld()->GetAuthGameMode()) : nullptr;
}

ASTU_PlayerState* USTU_GameDataWidget::GetPlayerState() const
{
	return GetOwningPlayer() ? GetOwningPlayer()->GetPlayerState<ASTU_PlayerState>() : nullptr;
}
