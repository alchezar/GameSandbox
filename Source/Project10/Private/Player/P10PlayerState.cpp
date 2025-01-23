// Copyright (C) 2023, IKinder

#include "Player/P10PlayerState.h"

void AP10PlayerState::AddScore(const float ScoreDelta)
{
	SetScore(GetScore() + ScoreDelta);
}

void AP10PlayerState::ResetScore()
{
	SetScore(0.f);
}
