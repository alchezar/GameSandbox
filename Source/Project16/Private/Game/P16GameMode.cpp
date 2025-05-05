// Copyright © 2025, Ivan Kinder

#include "Game/P16GameMode.h"

#include "Project16.h"
#include "Game/P16LoadScreenSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ViewModel/P16MVVMLoadSlot.h"

AP16GameMode::AP16GameMode()
{}

void AP16GameMode::BeginPlay()
{
	Super::BeginPlay();

	if (!Maps.IsEmpty())
	{
		DefaultMapName = Maps.Array()[0].Key;
	}
}

void AP16GameMode::SaveSlotData(const UP16MVVMLoadSlot* LoadSlot, const int32 SlotIndex) const
{
	// Get data from MVVM load slot, if valid.
	EARLY_RETURN_IF(!LoadSlot)
	const FString& SlotName = LoadSlot->GetLoadSlotName();

	// Delete saved game slot if exists.
	DeleteSlot(SlotName, SlotIndex);

	// Fill save data.
	USaveGame* SaveGameObject = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	EARLY_RETURN_IF(!SaveGameObject)
	UP16LoadScreenSaveGame* LoadScreenSaveGame = Cast<UP16LoadScreenSaveGame>(SaveGameObject);
	EARLY_RETURN_IF(!LoadScreenSaveGame)
	LoadScreenSaveGame->PlayerName = LoadSlot->GetPlayerName();
	LoadScreenSaveGame->MapName    = LoadSlot->GetMapName();
	LoadScreenSaveGame->SlotStatus = EP16SaveSlotStatus::Taken;
	LoadScreenSaveGame->Level      = LoadSlot->GetPlayerLevel();

	// Save data.
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, SlotName, SlotIndex);
}

void AP16GameMode::DeleteSlot(const FString& SlotName, const int32 SlotIndex)
{
	EARLY_RETURN_IF(!UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
}

UP16LoadScreenSaveGame* AP16GameMode::GetSavedSlotData(const FString& SlotName, const int32 SlotIndex) const
{
	USaveGame* SaveGameObject = UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex)
		? UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex)
		: UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);

	return Cast<UP16LoadScreenSaveGame>(SaveGameObject);
}

void AP16GameMode::TravelToMap(const UP16MVVMLoadSlot* LoadSlot)
{
	const FString MapName = LoadSlot->GetMapName();
	EARLY_RETURN_IF(!Maps.Contains(MapName))

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Maps[MapName]);
}
