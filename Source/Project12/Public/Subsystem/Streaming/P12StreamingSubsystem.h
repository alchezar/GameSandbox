// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12StreamingSubsystemVolume.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "P12StreamingSubsystem.generated.h"

class UP12StreamingSubsystemManager;

UCLASS()
class PROJECT12_API UP12StreamingSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual UWorld* GetWorld() const override;
	void OnVolumeBeginOverlap(AP12StreamingSubsystemVolume* SubsystemVolume);
	void OnVolumeOverlapEnd(AP12StreamingSubsystemVolume* SubsystemVolume);
	bool GetCanUseSubsystem();

private:
	void CreateStreamingLevelManagers(UWorld* World);
	void RemoveStreamingLevelManagers();
	void OnPreLoadMap(const FString& MapName);
	void OnPostLoadMapWithWorld(UWorld* LoadedWorld);
	bool FindStreamingLevelManager(const FString& LevelName, UP12StreamingSubsystemManager*& LevelManager);

private:
	UPROPERTY(Transient)
	TMap<FString, UP12StreamingSubsystemManager*> StreamingLevelManagers;
};
