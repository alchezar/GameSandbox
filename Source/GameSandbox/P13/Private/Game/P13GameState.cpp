// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameState.h"

AP13GameState::AP13GameState()
{
	
}

void AP13GameState::BeginPlay()
{
	Super::BeginPlay();
}

void AP13GameState::GoToNextPhase()
{
	int32 PhaseInt = StaticCast<int32>(LevelPhase);
	LevelPhase = TEnumAsByte<EP13LevelPhase>(++PhaseInt);
}
