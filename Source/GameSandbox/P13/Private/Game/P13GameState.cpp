// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameState.h"

AP13GameState::AP13GameState()
{}

void AP13GameState::BeginPlay()
{
	Super::BeginPlay();

	LevelPhase = StartLevelPhase;
}

void AP13GameState::GoToNextPhase()
{
	LevelPhase = StaticCast<EP13LevelPhase>((StaticCast<uint8>(LevelPhase) + 1) % StaticCast<uint8>(EP13LevelPhase::MAX));
}
