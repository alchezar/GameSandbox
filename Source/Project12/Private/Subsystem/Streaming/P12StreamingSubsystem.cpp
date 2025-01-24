// Copyright (C) 2023, IKinder

#include "Subsystem/Streaming/P12StreamingSubsystem.h"

#include "Subsystem/Streaming/P12StreamingSubsystemManager.h"

void UP12StreamingSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystem::Initialize(): %s"), *GetNameSafe(this));

	FCoreUObjectDelegates::PreLoadMap.AddUObject(this, &ThisClass::OnPreLoadMap);
	FCoreUObjectDelegates::PostLoadMapWithWorld.AddUObject(this, &ThisClass::OnPostLoadMapWithWorld);

	CreateStreamingLevelManagers(GetWorld());
}

void UP12StreamingSubsystem::Deinitialize()
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystem::Deinitialize(): %s"), *GetNameSafe(this));

	FCoreUObjectDelegates::PreLoadMap.RemoveAll(this);
	FCoreUObjectDelegates::PostLoadMapWithWorld.RemoveAll(this);

	RemoveStreamingLevelManagers();

	Super::Deinitialize();
}

UWorld* UP12StreamingSubsystem::GetWorld() const
{
	if (const UGameInstance* GameInstance = GetGameInstance())
	{
		return GameInstance->GetWorld();
	}
	if (const UObject* Outer = GetOuter())
	{
		return Outer->GetWorld();
	}
	return nullptr;
}

void UP12StreamingSubsystem::OnVolumeBeginOverlap(AP12StreamingSubsystemVolume* SubsystemVolume)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystem::OnVolumeBeginOverlap(): %s, SubsystemVolume: %s"), *GetNameSafe(this), *GetNameSafe(SubsystemVolume));

	for (const FString& LevelToLoad : SubsystemVolume->GetLevelsToLoad())
	{
		UP12StreamingSubsystemManager* Manager = nullptr;
		if (!FindStreamingLevelManager(LevelToLoad, Manager))
		{
			continue;
		}
		Manager->AddLoadRequest(SubsystemVolume);
	}

	for (const FString& LevelToUnload : SubsystemVolume->GetLevelsToUnload())
	{
		UP12StreamingSubsystemManager* Manager = nullptr;
		if (!FindStreamingLevelManager(LevelToUnload, Manager))
		{
			continue;
		}
		Manager->AddUnloadRequest(SubsystemVolume);
	}
}

void UP12StreamingSubsystem::OnVolumeOverlapEnd(AP12StreamingSubsystemVolume* SubsystemVolume)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystem::OnVolumeOverlapEnd(): %s, SubsystemVolume: %s"), *GetNameSafe(this), *GetNameSafe(SubsystemVolume));

	for (const FString& LevelToLoad : SubsystemVolume->GetLevelsToLoad())
	{
		UP12StreamingSubsystemManager* Manager = nullptr;
		if (!FindStreamingLevelManager(LevelToLoad, Manager))
		{
			continue;
		}
		Manager->RemoveLoadRequest(SubsystemVolume);
	}

	for (const FString& LevelToUnload : SubsystemVolume->GetLevelsToUnload())
	{
		UP12StreamingSubsystemManager* Manager = nullptr;
		if (!FindStreamingLevelManager(LevelToUnload, Manager))
		{
			continue;
		}
		Manager->RemoveUnloadRequest(SubsystemVolume);
	}
}

bool UP12StreamingSubsystem::GetCanUseSubsystem()
{
	return !StreamingLevelManagers.IsEmpty();
}

void UP12StreamingSubsystem::CreateStreamingLevelManagers(UWorld* World)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystem::RemoveStreamingLevelManager(): %s, World: %s"), *GetNameSafe(this), *GetNameSafe(World));

	RemoveStreamingLevelManagers();

	if (!World)
	{
		return;
	}

	const TArray<ULevelStreaming*>& StreamingLevels = World->GetStreamingLevels();
	StreamingLevelManagers.Reserve(StreamingLevels.Num());
	for (ULevelStreaming* Level : StreamingLevels)
	{
		FString ShortLevelName = FPackageName::GetShortName(Level->GetWorldAssetPackageName());
		if (ShortLevelName.StartsWith(World->StreamingLevelsPrefix))
		{
			ShortLevelName = ShortLevelName.RightChop(World->StreamingLevelsPrefix.Len());
		}
		UP12StreamingSubsystemManager* Manager = NewObject<UP12StreamingSubsystemManager>(this);
		Manager->Initialize(Level, ShortLevelName);

		StreamingLevelManagers.Add(ShortLevelName, Manager);
	}
}

void UP12StreamingSubsystem::RemoveStreamingLevelManagers()
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystem::RemoveStreamingLevelManager(): %s"), *GetNameSafe(this));

	for (auto& [LevelName, Manager] : StreamingLevelManagers)
	{
		if (!Manager)
		{
			continue;
		}
		Manager->Deinitialize();
	}
	StreamingLevelManagers.Empty();
}

void UP12StreamingSubsystem::OnPreLoadMap(const FString& MapName)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystem::OnPreLoadMap(): %s, MapName: %s"), *GetNameSafe(this), *MapName);

	RemoveStreamingLevelManagers();
}

void UP12StreamingSubsystem::OnPostLoadMapWithWorld(UWorld* LoadedWorld)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystem::OnPreLoadMap(): %s, LoadedWorld: %s"), *GetNameSafe(this), *GetNameSafe(LoadedWorld));

	CreateStreamingLevelManagers(LoadedWorld);
}

bool UP12StreamingSubsystem::FindStreamingLevelManager(const FString& LevelName, UP12StreamingSubsystemManager*& LevelManager)
{
	UP12StreamingSubsystemManager** ManagerPtr = StreamingLevelManagers.Find(LevelName);
	if (!ManagerPtr || !*ManagerPtr)
	{
		LevelManager = nullptr;
		return false;
	}
	LevelManager = *ManagerPtr;
	return true;
}
