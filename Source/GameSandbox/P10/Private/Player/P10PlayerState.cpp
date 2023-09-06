// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10PlayerState.h"

void AP10PlayerState::AddScore(const float ScoreDelta)
{
	SetScore(GetScore() + ScoreDelta);
}
