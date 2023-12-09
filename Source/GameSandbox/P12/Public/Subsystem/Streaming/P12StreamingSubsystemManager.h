// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelStreaming.h"
#include "UObject/Object.h"
#include "P12StreamingSubsystemManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogP12StreamingSubsystem, Log, All)

class AP12StreamingSubsystemVolume;

UCLASS()
class GAMESANDBOX_API UP12StreamingSubsystemManager : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE ELevelStreamingState GetStreamLevelState() const { return StreamingLevelState; }
	void Initialize(ULevelStreaming* InStreamingLevel, const FString& InLevelName);
	void Deinitialize();
	void AddLoadRequest(AP12StreamingSubsystemVolume* Volume);
	void RemoveLoadRequest(AP12StreamingSubsystemVolume* Volume);
	void AddUnloadRequest(AP12StreamingSubsystemVolume* Volume);
	void RemoveUnloadRequest(AP12StreamingSubsystemVolume* Volume);

private:
	void LoadLevel();
	void UnloadLevel();

	UFUNCTION()
	void OnLevelLoaded();
	UFUNCTION()
	void OnLevelUnloaded();

private:
	FString LevelName;
	ELevelStreamingState StreamingLevelState = ELevelStreamingState::Unloaded;
	TWeakObjectPtr<ULevelStreaming> StreamingLevel;
	TArray<TWeakObjectPtr<AP12StreamingSubsystemVolume>> LoadRequests;
	TArray<TWeakObjectPtr<AP12StreamingSubsystemVolume>> UnloadRequests;
};
