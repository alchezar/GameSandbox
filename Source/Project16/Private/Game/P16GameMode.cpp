// Copyright © 2025, Ivan Kinder

#include "Game/P16GameMode.h"

#include "EngineUtils.h"
#include "Project16.h"
#include "Game/P16GameInstance.h"
#include "Game/P16SaveGame.h"
#include "GameFramework/PlayerStart.h"
#include "Interface/P16SaveInterface.h"
#include "Kismet/GameplayStatics.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "UI/ViewModel/P16MVVMLoadSlot.h"
#include "Util/P16Log.h"

AP16GameMode::AP16GameMode()
{}

AActor* AP16GameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const UP16GameInstance* GameInstance = GetGameInstance<UP16GameInstance>();
	EARLY_RETURN_VALUE_IF(!GameInstance, nullptr)
	const FName PlayerStartTag = GameInstance->PlayerStartTag;

	TArray<APlayerStart*> PlayerStarts;
	for (TActorIterator<APlayerStart> It {GetWorld()}; It; ++It)
	{
		const APlayerStart* PlayerStart = *It;
		CONTINUE_IF(PlayerStart->PlayerStartTag != PlayerStartTag)

		PlayerStarts.Add(*It);
	}
	EARLY_RETURN_VALUE_IF(PlayerStarts.IsEmpty(), nullptr)

	APlayerStart** StartPosition = PlayerStarts.FindByPredicate([PlayerStartTag](const APlayerStart* Start) -> bool
	{
		return Start->PlayerStartTag == PlayerStartTag;
	});

	return StartPosition ? *StartPosition : PlayerStarts[0];
}

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
	USaveGame* SaveGame = UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);
	EARLY_RETURN_IF(!SaveGame)
	UP16SaveGame* LoadScreenSaveGame = Cast<UP16SaveGame>(SaveGame);
	EARLY_RETURN_IF(!LoadScreenSaveGame)

	// Save Game object and write data to disk.
	LoadScreenSaveGame->SaveGameObject(LoadSlot, SlotIndex);
	UGameplayStatics::SaveGameToSlot(LoadScreenSaveGame, SlotName, SlotIndex);
}

void AP16GameMode::DeleteSlot(const FString& SlotName, const int32 SlotIndex)
{
	EARLY_RETURN_IF(!UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex))
	UGameplayStatics::DeleteGameInSlot(SlotName, SlotIndex);
}

UP16SaveGame* AP16GameMode::GetSavedSlotData(const FString& SlotName, const int32 SlotIndex) const
{
	USaveGame* SaveGameObject = UGameplayStatics::DoesSaveGameExist(SlotName, SlotIndex)
		? UGameplayStatics::LoadGameFromSlot(SlotName, SlotIndex)
		: UGameplayStatics::CreateSaveGameObject(LoadScreenSaveGameClass);

	return Cast<UP16SaveGame>(SaveGameObject);
}

UP16SaveGame* AP16GameMode::GetInGameSaveData() const
{
	const UP16GameInstance* GameInstance = GetGameInstance<UP16GameInstance>();
	return GetSavedSlotData(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex);
}

void AP16GameMode::SaveInGameProgress(UP16SaveGame* SaveGame) const
{
	UP16GameInstance* GameInstance = GetGameInstance<UP16GameInstance>();
	GameInstance->PlayerStartTag   = SaveGame->GameObject.PlayerStartTag;

	UGameplayStatics::SaveGameToSlot(SaveGame, GameInstance->LoadSlotName, GameInstance->LoadSlotIndex);
}

void AP16GameMode::TravelToMap(const UP16MVVMLoadSlot* LoadSlot)
{
	const FString MapName = LoadSlot->GetMapName();
	EARLY_RETURN_IF(!Maps.Contains(MapName))

	UGameplayStatics::OpenLevelBySoftObjectPtr(this, Maps[MapName]);
}

void AP16GameMode::SaveWorldState(UWorld* InWorld) const
{
	FString WorldName = InWorld->GetMapName();
	WorldName.RemoveFromStart(InWorld->StreamingLevelsPrefix);

	UP16GameInstance* GameInstance = GetGameInstance<UP16GameInstance>();
	EARLY_RETURN_IF(!GameInstance)

	UP16SaveGame* SaveGame = GetSavedSlotData(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex);
	EARLY_RETURN_IF(!SaveGame)

	if (!SaveGame->GetHasSaveMap(WorldName))
	{
		FP16SaveMap NewSaveMap;
		NewSaveMap.AssetName = WorldName;
		SaveGame->SavedMaps.Add(NewSaveMap);
	}

	FP16SaveMap SaveMap = SaveGame->GetSaveMapBy(WorldName);
	SaveMap.SavedActors.Empty();
	for (TActorIterator<AActor> It {InWorld}; It; ++It)
	{
		AActor* Actor = *It;
		CONTINUE_IF(!Actor || !Actor->Implements<UP16SaveInterface>())

		FP16SaveActor SavedActor = {Actor->GetName(), Actor->GetTransform()};
		FMemoryWriter MemoryWriter {SavedActor.Bytes};
		SerializeBytes(MemoryWriter, Actor);

		SaveMap.SavedActors.AddUnique(SavedActor);
	}

	// Replace old save map with the fresh one.
	for (FP16SaveMap& MapToReplace : SaveGame->SavedMaps)
	{
		CONTINUE_IF(MapToReplace.AssetName != WorldName)
		MapToReplace = SaveMap;
	}

	UGameplayStatics::SaveGameToSlot(SaveGame, GameInstance->LoadSlotName, GameInstance->LoadSlotIndex);
}

void AP16GameMode::LoadWorldState(UWorld* InWorld) const
{
	FString WorldName = InWorld->GetMapName();
	WorldName.RemoveFromStart(InWorld->StreamingLevelsPrefix);

	const UP16GameInstance* GameInstance = GetGameInstance<UP16GameInstance>();
	EARLY_RETURN_IF(!GameInstance || !UGameplayStatics::DoesSaveGameExist(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex))

	const UP16SaveGame* SaveGame = Cast<UP16SaveGame>(UGameplayStatics::LoadGameFromSlot(GameInstance->LoadSlotName, GameInstance->LoadSlotIndex));
	EARLY_RETURN_WITH_WARN_IF(!SaveGame)

	for (TActorIterator<AActor> It {InWorld}; It; ++It)
	{
		AActor* Actor = *It;
		CONTINUE_IF(!Actor || !Actor->Implements<UP16SaveInterface>())

		FP16SaveMap SaveMap = SaveGame->GetSaveMapBy(WorldName);
		for (const FP16SaveActor& SavedActor : SaveMap.SavedActors)
		{
			CONTINUE_IF(SavedActor.Name != Actor->GetFName())

			if (IP16SaveInterface::Execute_GetShouldLoadTransform(Actor))
			{
				Actor->SetActorTransform(SavedActor.Transform);
			}

			FMemoryReader MemoryReader {SavedActor.Bytes};
			SerializeBytes(MemoryReader, Actor);

			IP16SaveInterface::Execute_LoadActor(Actor);
		}
	}
}

void AP16GameMode::SerializeBytes(FArchive& InArchive, AActor* InActor) const
{
	// Pass the array to fill with data from Actor
	FObjectAndNameAsStringProxyArchive Ar {InArchive, true};
	// Find only variables with UPROPERTY(SaveGame)
	Ar.ArIsSaveGame = true;
	// Convert actors SaveGame UPROPERTY to binary array
	InActor->Serialize(Ar);
}
