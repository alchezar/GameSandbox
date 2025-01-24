// Copyright © 2024, IKinder

#include "Game/P13PlayerState.h"

#include "Game/P13GameState.h"
#include "GameFramework/GameModeBase.h"
#include "Library/P13Types.h"

AP13PlayerState::AP13PlayerState() {}

void AP13PlayerState::BeginPlay()
{
	Super::BeginPlay();
	ResetLives();
}

bool AP13PlayerState::GetCanLoadPlayerInventory() const
{
	return !LastLifeWeaponSlots.IsEmpty() && !LastLifeAmmoSlots.IsEmpty();
}

bool AP13PlayerState::GetPlayerCanRespawnAfterDeath()
{
	OnPlayerDied();
	return GetPlayerCanRespawn();
}

bool AP13PlayerState::GetPlayerCanRespawn() const
{
	return TotalLives > 0;
}

void AP13PlayerState::OnPlayerDied()
{
	TotalLives = FMath::Max(--TotalLives, 0);
	OnLivesChanged.Broadcast(TotalLives);
}

TArray<FP13WeaponSlot> AP13PlayerState::GetSavedWeaponSlots()
{
	/* Guarantee, that we can't use saved slot more than one time. */
	TArray<FP13WeaponSlot> WeaponSlotsCopy = LastLifeWeaponSlots;
	LastLifeWeaponSlots.Empty();
	return WeaponSlotsCopy;
}

TArray<FP13AmmoSlot> AP13PlayerState::GetSavedAmmoSlots()
{
	/* Guarantee, that we can't use saved slot more than one time. */
	TArray<FP13AmmoSlot> AmmoSlotsCopy = LastLifeAmmoSlots;
	LastLifeAmmoSlots.Empty();
	return AmmoSlotsCopy;
}

void AP13PlayerState::SavePlayerInventory(const TArray<FP13WeaponSlot>& WeaponSlots, const TArray<FP13AmmoSlot>& AmmoSlots)
{
	LastLifeWeaponSlots = WeaponSlots;
	LastLifeAmmoSlots = AmmoSlots;
}

void AP13PlayerState::AddScore(const int32 NewScore)
{
	if (NewScore <= 0)
	{
		return;
	}
	TotalScore += NewScore;
	OnScoreChanged.Broadcast(TotalScore);

	if (AP13GameState* GameState = GetWorld()->GetAuthGameMode()->GetGameState<AP13GameState>())
	{
		GameState->CheckWinCondition(TotalScore);
	}
}

void AP13PlayerState::SaveTotalTime(const int32 LastLifeTime)
{
	TotalTime = LastLifeTime;
}

void AP13PlayerState::ResetLives()
{
	TotalLives = MaxLives;
	OnLivesChanged.Broadcast(TotalLives);
}
