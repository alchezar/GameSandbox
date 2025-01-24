// Copyright (C) 2023, IKinder

#include "Subsystem/Streaming/P12StreamingSubsystemManager.h"

#include "Subsystem/Streaming/P12StreamingSubsystemVolume.h"

DEFINE_LOG_CATEGORY(LogP12StreamingSubsystem)

void UP12StreamingSubsystemManager::Initialize(ULevelStreaming* InStreamingLevel, const FString& InLevelName)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::Initialize(): %s, StreamingLevel: %s, LevelName: %s"), *GetNameSafe(this), *GetNameSafe(InStreamingLevel), *InLevelName);

	if (!InStreamingLevel)
	{
		return;
	}
	LevelName = InLevelName;
	StreamingLevel = InStreamingLevel;
	StreamingLevelState = InStreamingLevel->GetLevelStreamingState();

	StreamingLevel->OnLevelShown.AddUniqueDynamic(this, &ThisClass::OnLevelLoaded);
	StreamingLevel->OnLevelHidden.AddUniqueDynamic(this, &ThisClass::OnLevelUnloaded);
}

void UP12StreamingSubsystemManager::Deinitialize()
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::Deinitialize()"));

	if (StreamingLevel.IsValid())
	{
		StreamingLevel->OnLevelShown.RemoveDynamic(this, &ThisClass::OnLevelLoaded);
		StreamingLevel->OnLevelHidden.RemoveDynamic(this, &ThisClass::OnLevelUnloaded);
	}
	StreamingLevel.Reset();
}

void UP12StreamingSubsystemManager::AddLoadRequest(AP12StreamingSubsystemVolume* Volume)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::AddLoadRequest(): LevelName: %s, SubsystemVolume: %s"), *LevelName, *GetNameSafe(Volume));

	if (LoadRequests.Contains(Volume))
	{
		return;
	}
	LoadRequests.AddUnique(Volume);

	if (LoadRequests.Num() == 1 && UnloadRequests.IsEmpty())
	{
		LoadLevel();
	}
}

void UP12StreamingSubsystemManager::RemoveLoadRequest(AP12StreamingSubsystemVolume* Volume)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::RemoveLoadRequest(): LevelName: %s, SubsystemVolume: %s"), *LevelName, *GetNameSafe(Volume));

	if (!LoadRequests.Contains(Volume))
	{
		return;
	}
	LoadRequests.Remove(Volume);

	if (LoadRequests.IsEmpty() && UnloadRequests.IsEmpty())
	{
		UnloadLevel();
	}
}

void UP12StreamingSubsystemManager::AddUnloadRequest(AP12StreamingSubsystemVolume* Volume)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::AddUnloadRequest(): LevelName: %s, SubsystemVolume: %s"), *LevelName, *GetNameSafe(Volume));

	if (UnloadRequests.Contains(Volume))
	{
		return;
	}
	UnloadRequests.AddUnique(Volume);

	if (UnloadRequests.Num() == 1 && !LoadRequests.IsEmpty())
	{
		UnloadLevel();
	}
}

void UP12StreamingSubsystemManager::RemoveUnloadRequest(AP12StreamingSubsystemVolume* Volume)
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::AddUnloadRequest(): LevelName: %s, SubsystemVolume: %s"), *LevelName, *GetNameSafe(Volume));

	if (!UnloadRequests.Contains(Volume))
	{
		return;
	}
	UnloadRequests.Remove(Volume);

	if (UnloadRequests.IsEmpty() && !LoadRequests.IsEmpty())
	{
		LoadLevel();
	}
}

void UP12StreamingSubsystemManager::LoadLevel()
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::LoadLevel(): LevelName: %s"), *LevelName);

	if (!StreamingLevel.IsValid())
	{
		return;
	}

	StreamingLevel->SetShouldBeLoaded(true);
	StreamingLevel->SetShouldBeVisible(true);
	StreamingLevel->bShouldBlockOnLoad = true;
}

void UP12StreamingSubsystemManager::UnloadLevel()
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::UnloadLevel(): LevelName: %s"), *LevelName);

	if (!StreamingLevel.IsValid())
	{
		return;
	}

	StreamingLevel->SetShouldBeLoaded(false);
	StreamingLevel->SetShouldBeVisible(false);
	StreamingLevel->bShouldBlockOnLoad = true;
}

void UP12StreamingSubsystemManager::OnLevelLoaded()
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::OnLevelLoaded(): LevelName: %s"), *LevelName);

	if (!StreamingLevel.IsValid())
	{
		return;
	}
	StreamingLevelState = StreamingLevel->GetLevelStreamingState();
}

void UP12StreamingSubsystemManager::OnLevelUnloaded()
{
	UE_LOG(LogP12StreamingSubsystem, Display, TEXT("UP12StreamingSubsystemManager::OnLevelUnloaded(): LevelName: %s"), *LevelName);

	if (!StreamingLevel.IsValid())
	{
		return;
	}
	StreamingLevelState = StreamingLevel->GetLevelStreamingState();
}
