// Copyright © 2024, IKinder

#include "P13/Public/Game/P13PlayerState.h"

AP13PlayerState::AP13PlayerState()
{
	
}

void AP13PlayerState::BeginPlay()
{
	Super::BeginPlay();
	ResetLives();
}

bool AP13PlayerState::GetPlayerCanRespawnAfterDeath()
{
	OnPlayerDied();
	return GetPlayerCanRespawn();
}

bool AP13PlayerState::GetPlayerCanRespawn() const
{
	return CurrentLives > 0;
}

void AP13PlayerState::OnPlayerDied()
{
	CurrentLives = FMath::Max(--CurrentLives, 0);
}

void AP13PlayerState::ResetLives()
{
	CurrentLives = MaxLives;
}
