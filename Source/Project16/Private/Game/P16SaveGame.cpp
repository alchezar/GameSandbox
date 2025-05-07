// Copyright © 2025, Ivan Kinder

#include "Game/P16SaveGame.h"

#include "Project16.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "Player/P16PlayerState.h"
#include "UI/ViewModel/P16MVVMLoadSlot.h"

void UP16SaveGame::SaveGameObject(const UP16MVVMLoadSlot* LoadSlot, const int32 SlotIndex)
{
	GameObject.SlotName       = LoadSlot->GetLoadSlotName();
	GameObject.SlotIndex      = SlotIndex;
	GameObject.PlayerName     = LoadSlot->GetPlayerName();
	GameObject.MapName        = LoadSlot->GetMapName();
	GameObject.SlotStatus     = EP16SaveSlotStatus::Taken;
	GameObject.PlayerStartTag = LoadSlot->PlayerStartTag;
}

void UP16SaveGame::SavePlayerObject(const APlayerState* InPlayerState)
{
	const AP16PlayerState* PlayerState = Cast<AP16PlayerState>(InPlayerState);
	EARLY_RETURN_IF(!PlayerState)

	PlayerObject.Level           = PlayerState->GetPlayerLevel();
	PlayerObject.XP              = PlayerState->GetXP();
	PlayerObject.SpellPoints     = PlayerState->GetSpellPoints();
	PlayerObject.AttributePoints = PlayerState->GetAttributePoints();

	const UP16AttributeSet* AttributeSet = Cast<UP16AttributeSet>(PlayerState->GetAttributeSet());
	EARLY_RETURN_IF(!AttributeSet)

	PlayerObject.Strength     = UP16AttributeSet::GetStrengthAttribute().GetNumericValue(AttributeSet);
	PlayerObject.Intelligence = UP16AttributeSet::GetIntelligenceAttribute().GetNumericValue(AttributeSet);
	PlayerObject.Resilience   = UP16AttributeSet::GetResilienceAttribute().GetNumericValue(AttributeSet);
	PlayerObject.Vigor        = UP16AttributeSet::GetVigorAttribute().GetNumericValue(AttributeSet);
}

void UP16SaveGame::LoadPlayerObject(APlayerState* InPlayerState)
{
	AP16PlayerState* PlayerState = Cast<AP16PlayerState>(InPlayerState);
	EARLY_RETURN_IF(!PlayerState)

	PlayerState->SetLevel(PlayerObject.Level);
	PlayerState->SetXP(PlayerObject.XP);
	PlayerState->SetSpellPoints(PlayerObject.SpellPoints);
	PlayerState->SetAttributePoints(PlayerObject.AttributePoints);

	// Primary attributes will set further in the character class.
}
