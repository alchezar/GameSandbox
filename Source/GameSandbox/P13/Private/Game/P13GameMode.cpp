// Copyright © 2024, IKinder

#include "P13/Public/Game/P13GameMode.h"

#include "P13/Public/Game/P13GameState.h"

AP13GameMode::AP13GameMode()
{}

void AP13GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AP13GameMode::Respawn(AController* NewPlayer)
{
	const AP13GameState* CurrentGameState = GetGameState<AP13GameState>();
	check(CurrentGameState)
	
	const EP13LevelPhase LastLevelPhase = CurrentGameState->GetLastLevelPhase();
	const FString LevelPhaseName = UEnum::GetValueAsString(LastLevelPhase).LeftChop(16);
	
	AActor* BestPlayerStart = FindPlayerStart(NewPlayer, LevelPhaseName);
	if (!BestPlayerStart)
	{
		RestartPlayer(NewPlayer);
	}
	RestartPlayerAtPlayerStart(NewPlayer, BestPlayerStart);
}
