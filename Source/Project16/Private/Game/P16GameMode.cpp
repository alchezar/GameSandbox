// Copyright © 2025, Ivan Kinder

#include "Game/P16GameMode.h"

#include "Project16.h"
#include "Game/P16LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/P16MVVMLoadSlot.h"

void AP16GameMode::SaveSlotData(const UP16MVVMLoadSlot* LoadSlot, const int32 SlotIndex)
{
	// Get data from MVVM load slot, if valid.
	EARLY_RETURN_IF(!LoadSlot)
	const FString& PlayerName = LoadSlot->GetPlayerName();
	const FString& SlotName   = LoadSlot->GetLoadSlotName();

	// Delete saved game slot if exists.
	if (UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	{
		UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
	}

	// Fill save data.
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	EARLY_RETURN_IF(!SaveGameObject)
	UP16LoadScreenSaveGame* LoadScreenSaveGame = Cast<UP16LoadScreenSaveGame>(SaveGameObject);
	EARLY_RETURN_IF(!LoadScreenSaveGame)
	LoadScreenSaveGame->PlayerName = PlayerName;
	LoadScreenSaveGame->SlotStatus = EP16SaveSlotStatus::Taken;

	// Save data.
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, SlotName, SlotIndex);
}

UP16LoadScreenSaveGame* AP16GameMode::GetSavedSlotData(const FString& SlotName, const int32 SlotIndex) const
{
	USaveGame* SaveGameObject = UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex)
		? UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex)
		: UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);

	return Cast<UP16LoadScreenSaveGame>(SaveGameObject);
}
