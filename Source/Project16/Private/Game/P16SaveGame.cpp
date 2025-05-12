// Copyright © 2025, Ivan Kinder

#include "Game/P16SaveGame.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AbilitySystemLibrary.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "AbilitySystem/Data/P16AbilityInfoDataAsset.h"
#include "Character/P16Character.h"
#include "Player/P16PlayerState.h"
#include "UI/ViewModel/P16MVVMLoadSlot.h"

FP16SaveMap UP16SaveGame::GetSaveMapBy(const FString& InMapName) const
{
	const FP16SaveMap* Result = SavedMaps.FindByPredicate([InMapName](const FP16SaveMap& MapSave) -> bool
	{
		return MapSave.AssetName == InMapName;
	});

	return Result ? *Result : FP16SaveMap {};
}

bool UP16SaveGame::GetHasSaveMap(const FString& InMapName) const
{
	return SavedMaps.ContainsByPredicate([InMapName](const FP16SaveMap& MapSave) -> bool
	{
		return MapSave.AssetName == InMapName;
	});
}

void UP16SaveGame::SaveProgress(const FName& CheckpointTag, const AP16Character* InCharacter)
{
	GameObject.PlayerStartTag = CheckpointTag;
	GameObject.bFirstLoad     = false;

	SavePlayerObject(InCharacter->GetPlayerState());
	SaveAbilityObject(InCharacter->GetAbilitySystemComponent());
}

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

void UP16SaveGame::SaveAbilityObject(UAbilitySystemComponent* InAbilitySystemComponent)
{
	UP16AbilitySystemComponent* AbilitySystemComponent = Cast<UP16AbilitySystemComponent>(InAbilitySystemComponent);
	EARLY_RETURN_IF(!AbilitySystemComponent)

	SavedAbilities.Empty();

	FP16ForEachAbilitySignature SaveAbilityDelegate;
	SaveAbilityDelegate.BindWeakLambda(AbilitySystemComponent, [this, AbilitySystemComponent](const FGameplayAbilitySpec& AbilitySpec) -> void
	{
		FGameplayTag              AbilityTag      = UP16AbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec);
		UP16AbilityInfoDataAsset* AbilityInfoData = UP16AbilitySystemLibrary::GetAbilityInfo(AbilitySystemComponent->GetAvatarActor());
		FP16AbilityInfo           AbilityInfo     = AbilityInfoData->FindAbilityInfo(AbilityTag);

		FP16SavedAbility SavedAbility = {
			.Ability = AbilityInfo.Ability,
			.Tag = AbilityTag,
			.Type = AbilityInfo.TypeTag,
			.Status = AbilitySystemComponent->GetStatusFromAbilityTag(AbilityTag),
			.Slot = AbilitySystemComponent->GetInputTagFromAbilityTag(AbilityTag),
			.Level = AbilitySpec.Level
		};

		SavedAbilities.Add(SavedAbility);
	});
	AbilitySystemComponent->ForEachAbility(SaveAbilityDelegate);
}

void UP16SaveGame::LoadGameObject(UP16MVVMLoadSlot* LoadSlot) const
{
	LoadSlot->SetPlayerName(GameObject.PlayerName);
	LoadSlot->SetMapName(GameObject.MapName);
	LoadSlot->SetLevelName(GameObject.LevelName);
	LoadSlot->SlotStatus     = GameObject.SlotStatus;
	LoadSlot->PlayerStartTag = GameObject.PlayerStartTag;

	LoadSlot->SetPlayerLevel(PlayerObject.Level);
	LoadSlot->InitSlot();
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

void UP16SaveGame::LoadAbilityObject(UAbilitySystemComponent* InAbilitySystemComponent)
{
	UP16AbilitySystemComponent* AbilitySystemComponent = Cast<UP16AbilitySystemComponent>(InAbilitySystemComponent);
	EARLY_RETURN_IF(!AbilitySystemComponent)

	AbilitySystemComponent->AddCharacterAbilitiesFromSaveData(this);
}
