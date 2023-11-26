// Copyright (C) 2023, IKinder

#include "P12/Public/Subsystem/SaveSubsystem/P12SaveSubsystem.h"

#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "P12/Public/Subsystem/SaveSubsystem/P12SaveSubsystemInterface.h"
#include "P12/Public/Subsystem/SaveSubsystem/P12SaveSubsystemTypes.h"
#include "P12/Public/Subsystem/SaveSubsystem/P12SaveSubsystemUtils.h"
#include "Serialization/ArchiveLoadCompressedProxy.h"
#include "Serialization/ArchiveSaveCompressedProxy.h"

void UP12SaveSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::Initialize(): %s"), *GetNameSafe(this));

	GameSaveData = FP12GameSaveData();
	SaveDirectoryName = *(FPaths::ProjectSavedDir() + "SaveGames/");

	FP12SaveDirectoryVisitor DirectoryVisitor = FP12SaveDirectoryVisitor(SaveIds);
	FPlatformFileManager::Get().GetPlatformFile().IterateDirectory(*SaveDirectoryName, DirectoryVisitor);
	SaveIds.Sort();

	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnPostLoadMapWithWorld);
}

void UP12SaveSubsystem::Deinitialize()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::Deinitialize(): %s"), *GetNameSafe(this));

	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);
	
	Super::Deinitialize();
}

void UP12SaveSubsystem::SerializeLevel(const ULevel* Level, const ULevelStreaming* LevelStreaming)
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::SerializeLevel(): %s, Level: %s, Streaming level: %s"), *GetNameSafe(this), *GetNameSafe(Level), *GetNameSafe(LevelStreaming));

	FP12LevelSaveData* LevelSaveData = &GameSaveData.Level;
	TArray<FP12ActorSaveData>& ActorsSaveData = LevelSaveData->ActorsSaveData;
	ActorsSaveData.Empty();

	for (AActor* Actor : Level->Actors)
	{
		if (!Actor || (Actor && !Actor->Implements<UP12SaveSubsystemInterface>()))
		{
			continue;
		}
		FP12ActorSaveData& ActorSaveData = ActorsSaveData[ActorsSaveData.AddUnique(FP12ActorSaveData(Actor))];
		ActorSaveData.Transform = Actor->GetTransform();

		auto MemoryWriter = FMemoryWriter(ActorSaveData.RawData, true);
		auto Archive = FP12SaveSubsystemArchive(MemoryWriter, false);
		Actor->Serialize(Archive);
	}
}

void UP12SaveSubsystem::DeserializeLevel(ULevel* Level, const ULevelStreaming* LevelStreaming)
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::DeserializeLevel(): %s, Level: %s, Streaming level: %s"), *GetNameSafe(this), *GetNameSafe(Level), *GetNameSafe(LevelStreaming));

	FP12LevelSaveData* LevelSaveData = &GameSaveData.Level;
	if (!LevelSaveData)
	{
		/* There is no save for this level yet. Call OnLevelDeserialized() in all cases! */
		UP12SaveSubsystemUtils::BroadcastOnLevelDeserialized(Level);
		return;
	}
	TArray<FP12ActorSaveData*> ActorsSaveData;
	ActorsSaveData.Reserve(LevelSaveData->ActorsSaveData.Num());
	for (FP12ActorSaveData& ActorSaveData : LevelSaveData->ActorsSaveData)
	{
		ActorsSaveData.Add(&ActorSaveData);
	}

	TArray<AActor*> ActorsToNotify;
	for (auto ActorIterator = Level->Actors.CreateIterator(); ActorIterator; ++ActorIterator)
	{
		AActor* Actor = *ActorIterator;
		if (!Actor || (Actor && !Actor->Implements<UP12SaveSubsystemInterface>()))
		{
			continue;
		}
		
		FP12ActorSaveData* ActorSaveData = nullptr;
		for (auto ActorSaveDataIterator = ActorsSaveData.CreateIterator(); ActorSaveDataIterator; ++ActorSaveDataIterator)
		{
			if ((*ActorSaveDataIterator)->Name != Actor->GetName())
			{
				continue;
			}
			ActorSaveData = *ActorSaveDataIterator;
			ActorSaveDataIterator.RemoveCurrent();
			break;
		}
		if (!ActorSaveData)
		{
			UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::DeserializeLevel(): %s, ActorSaveData not found!"), *GetNameSafe(this));
			Actor->Destroy();
			return;
		}

		DeserializeActor(Actor, *ActorSaveData);
		ActorsToNotify.Add(Actor);
	}
	FActorSpawnParameters ActorSpawnParameters;
	ActorSpawnParameters.OverrideLevel = Level;
	ActorSpawnParameters.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
	ActorSpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	for (FP12ActorSaveData* ActorSaveData : ActorsSaveData)
	{
		UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::DeserializeLevel(): %s, Spawn new actor with name: %s"), *GetNameSafe(this),  *ActorSaveData->Name.ToString());
		ActorSpawnParameters.Name = ActorSaveData->Name;

		/* bIgnoreOnActorSpawnedCallbackFast is hook to avoid double OnLevelDeserialized invocations. */
		FP12BoolScopeWrapper OnActorSpawnedHook(bIgnoreOnActorSpawnedCallback, true);

		AActor* Actor = GetWorld()->SpawnActor(ActorSaveData->Class.Get(), &ActorSaveData->Transform, ActorSpawnParameters);
		if (!Actor || (Actor && !Actor->Implements<UP12SaveSubsystemInterface>()))
		{
			UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::DeserializeLevel(): %s, Failed to spawn new actor with name: %s"), *GetNameSafe(this),  *ActorSaveData->Name.ToString());
			continue;
		}
		/* Actor name can be changed so update it. */
		ActorSaveData->Name = Actor->GetFName();

		DeserializeActor(Actor, *ActorSaveData);
		ActorsToNotify.Add(Actor);
	}

	for (AActor* Actor : ActorsToNotify)
	{
		IP12SaveSubsystemInterface::Execute_OnLevelDeserialized(Actor);
	}
}

void UP12SaveSubsystem::SaveGame()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::SaveGame(): %s"), *GetNameSafe(this));

	SerializeGame();
	WriteSaveToFile();
}

void UP12SaveSubsystem::LoadLastGame()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::LoadLastGame(): %s"), *GetNameSafe(this));

	if (SaveIds.IsEmpty())
	{
		UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::LoadLastGame(): %s Failed! No saves."), *GetNameSafe(this));
		return;
	}
	LoadGame(SaveIds[SaveIds.Last()]);
}

void UP12SaveSubsystem::LoadGame(const int32 SaveId)
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::LoadLastGame()"));

	if (!SaveIds.Contains(SaveId))
	{
		UE_LOG(LogP12SaveSubsystem, Warning, TEXT("UP12SaveSubsystem::LoadLastGame(): Failed!"));
		return;
	}
	LoadSaveFromFile(SaveId);
	UGameplayStatics::OpenLevel(this, GameSaveData.LevelName);
}

void UP12SaveSubsystem::SerializeGame()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::SerializeGame(): %s"), *GetNameSafe(this));

	UGameInstance* GameInstance = GetGameInstance();
	GameSaveData.GameInstance = FP12ObjectSaveData(GameInstance);

	auto MemoryWriter = FMemoryWriter(GameSaveData.GameInstance.RawData, true);
	auto Archive = FP12SaveSubsystemArchive(MemoryWriter, false);
	GameInstance->Serialize(Archive);

	FString LevelName = UGameplayStatics::GetCurrentLevelName(GetWorld());
	if (GetWorld()->IsPlayInEditor())
	{
		LevelName = UWorld::RemovePIEPrefix(LevelName);
	}
	GameSaveData.LevelName = FName(LevelName);

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (PlayerCharacter)
	{
		GameSaveData.StartTransform = PlayerCharacter->GetTransform();
	}
	SerializeLevel(GetWorld()->PersistentLevel);
}

void UP12SaveSubsystem::DeserializeGame()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::DeserializeGame(): %s"), *GetNameSafe(this));

	UGameInstance* GameInstance = GetGameInstance();
	auto MemoryReader = FMemoryReader(GameSaveData.GameInstance.RawData, true);
	auto Archive = FP12SaveSubsystemArchive(MemoryReader, false);
	GameInstance->Serialize(Archive);

	DeserializeLevel(GetWorld()->PersistentLevel);
}

void UP12SaveSubsystem::WriteSaveToFile()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::WriteSaveToFile(): %s"), *GetNameSafe(this));

	const int32 SaveId = GetNextSaveId();
	SaveIds.AddUnique(SaveId);
	GameSaveData.Name = FName(FString::FromInt(SaveId));

	TArray<uint8> SaveBytes;
	auto MemoryWriter = FMemoryWriter(SaveBytes);
	auto WriterArchive = FObjectAndNameAsStringProxyArchive(MemoryWriter, false);
	GameSaveData.Serialize(WriterArchive);

	FArchive* FileWriter = IFileManager::Get().CreateFileWriter(*GetSaveFilePath(SaveId));

	if (bUseCompressedSaves)
	{
		TArray<uint8> CompressedSaveBytes;
		auto CompressedArchive = FArchiveSaveCompressedProxy(CompressedSaveBytes, NAME_Zlib);
		CompressedArchive << SaveBytes;
		CompressedArchive.Flush();

		*FileWriter << CompressedSaveBytes;
	}
	else
	{
		*FileWriter << SaveBytes;
	}

	FileWriter->Close();
	delete FileWriter;
}

void UP12SaveSubsystem::LoadSaveFromFile(int32 SaveId)
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::LoadSaveFromFile(): %s, SaveID: %i"), *GetNameSafe(this), SaveId);

	FArchive* FileReader = IFileManager::Get().CreateFileReader(*GetSaveFilePath(SaveId));

	TArray<uint8> SaveBytes;
	if (bUseCompressedSaves)
	{
		TArray<uint8> CompressedSaveBytes;
		*FileReader << CompressedSaveBytes;

		auto DecompressedArchive = FArchiveLoadCompressedProxy(CompressedSaveBytes, NAME_Zlib);
		DecompressedArchive << SaveBytes;
		DecompressedArchive.Flush();
	}
	else
	{
		*FileReader << SaveBytes;
	}

	FileReader->Close();
	delete FileReader;
}

void UP12SaveSubsystem::OnPostLoadMapWithWorld(UWorld* LoadedWorld)
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::OnPostLoadMapWithWorld(): %s, World: %s"), *GetNameSafe(this), *GetNameSafe(LoadedWorld));

	DeserializeGame();
}

void UP12SaveSubsystem::DeserializeActor(AActor* Actor, const FP12ActorSaveData& ActorSaveData)
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::DeserializeActor(): %s, Actor: %s"), *GetNameSafe(this), *GetNameSafe(Actor));

	Actor->SetActorTransform(ActorSaveData.Transform);

	auto MemoryReader = FMemoryReader(ActorSaveData.RawData, true);
	auto Archive = FP12SaveSubsystemArchive(MemoryReader, false);
	Actor->Serialize(Archive);
}

FString UP12SaveSubsystem::GetSaveFilePath(int32 SaveId) const
{
	return SaveDirectoryName + "/" + FString::FromInt(SaveId) + ".save";
}

int32 UP12SaveSubsystem::GetNextSaveId() const
{
	if (SaveIds.IsEmpty())
	{
		return 1;
	}
	return SaveIds[SaveIds.Last()] + 1;
}

void UP12SaveSubsystem::OnActorSpawned(AActor* SpawnedActor)
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::OnActorSpawned(): %s, Actor: %s"), *GetNameSafe(this), *GetNameSafe(SpawnedActor));

	if (bIgnoreOnActorSpawnedCallback)
	{
		UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12SaveSubsystem::OnActorSpawned(): %s, Skipped for actor %s. bIgnoreOnActorSpawnedCallback == true!"), *GetNameSafe(this), *GetNameSafe(SpawnedActor));
		return;
	}
	if (!SpawnedActor || (SpawnedActor && !SpawnedActor->Implements<UP12SaveSubsystemInterface>()))
	{
		return;
	}
	/* We should notify runtime spawned actors too. */
	IP12SaveSubsystemInterface::Execute_OnLevelDeserialized(SpawnedActor);
}
