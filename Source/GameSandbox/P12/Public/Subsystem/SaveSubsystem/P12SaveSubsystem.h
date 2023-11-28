// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12SaveData.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "P12SaveSubsystem.generated.h"

class UP12StreamingLevelObserver;

UCLASS()
class GAMESANDBOX_API UP12SaveSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual UWorld* GetWorld() const override;
	void SerializeLevel(const ULevel* Level, const ULevelStreaming* StreamingLevel = nullptr);
	void DeserializeLevel(ULevel* Level, const ULevelStreaming* StreamingLevel = nullptr);
	
	FORCEINLINE const FP12GameSaveData& GetGameSaveData() const { return GameSaveData; }
	UFUNCTION(BlueprintCallable, Category = "C++ | SaveSubsystem")
	void SaveGame();
	UFUNCTION(BlueprintCallable, Category = "C++ | SaveSubsystem")
	void LoadLastGame();
	UFUNCTION(BlueprintCallable, Category = "C++ | SaveSubsystem")
	void LoadGame(const int32 SaveId);
	
private:
	void SerializeGame();
	void DeserializeGame();
	void WriteSaveToFile();
	void LoadSaveFromFile(int32 SaveId);
	void OnPostLoadMapWithWorld(UWorld* LoadedWorld);
	void DeserializeActor(AActor* Actor, const FP12ActorSaveData* ActorSaveData);
	FString GetSaveFilePath(int32 SaveId) const;
	int32 GetNextSaveId() const;
	void OnActorSpawned(AActor* SpawnedActor);
	
	void SerializeActorComponents(AActor* Actor, FP12ActorSaveData& ActorSaveData);
	void DeserializeActorComponents(AActor* Actor, const FP12ActorSaveData* ActorSaveData);
	bool GetCanBeSaved(const UObject* Object);
	void NotifyActorAndComponents(AActor* Actor);

	void CreateStreamingLevelObserver(UWorld* World);
	void RemoveStreamingLevelObserver();
	
private:
	FP12GameSaveData GameSaveData;
	FString SaveDirectoryName;
	TArray<int32> SaveIds;
	FDelegateHandle OnActorSpawnedDelegateHandle;
	
	bool bUseCompressedSaves = false;
	/* To avoid double ::OnLevelDeserialized() invocations */
	bool bIgnoreOnActorSpawnedCallback = false;

	UPROPERTY(Transient)
	TArray<UP12StreamingLevelObserver*> StreamingLevelObservers;
	
};
