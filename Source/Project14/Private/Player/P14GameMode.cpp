// Copyright © 2024, IKinder

#include "Player/P14GameMode.h"

AP14GameMode::AP14GameMode() {}

bool AP14GameMode::SetPause(APlayerController* PC, const FCanUnpause CanUnpauseDelegate)
{
	const bool bResult = Super::SetPause(PC, CanUnpauseDelegate);
	if (bResult)
	{
		OnGamePause.Broadcast(true);
	}

	return bResult;
}

bool AP14GameMode::ClearPause()
{
	const bool bResult = Super::ClearPause();
	if (bResult)
	{
		OnGamePause.Broadcast(false);
	}

	return bResult;
}
