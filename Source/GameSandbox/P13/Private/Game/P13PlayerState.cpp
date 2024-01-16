// Copyright © 2024, IKinder

#include "P13/Public/Game/P13PlayerState.h"

#include "P13/Public/Library/P13Types.h"

AP13PlayerState::AP13PlayerState()
{}

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
	return CurrentLives > 0;
}

void AP13PlayerState::OnPlayerDied()
{
	CurrentLives = FMath::Max(--CurrentLives, 0);
}

TArray<FP13WeaponSlot> AP13PlayerState::GetSavedWeaponSlots()
{
	/* Guarantee, that we can't use saved slot more than one time. */
	TArray<FP13WeaponSlot> WeaponSlotsCopy = LastLifeWeaponSlots;
	LastLifeWeaponSlots.Empty();
	return WeaponSlotsCopy ;
}

TArray<FP13AmmoSlot> AP13PlayerState::GetSavedAmmoSlots()
{
	/* Guarantee, that we can't use saved slot more than one time. */
	TArray<FP13AmmoSlot> AmmoSlotsCopy = LastLifeAmmoSlots;
	LastLifeAmmoSlots.Empty();
	return AmmoSlotsCopy ;
}

void AP13PlayerState::SavePlayerInventory(const TArray<FP13WeaponSlot>& WeaponSlots, const TArray<FP13AmmoSlot>& AmmoSlots)
{
	LastLifeWeaponSlots = WeaponSlots;
	LastLifeAmmoSlots = AmmoSlots;
}

void AP13PlayerState::ResetLives()
{
	CurrentLives = MaxLives;
}
