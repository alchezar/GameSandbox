// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/StrongObjectPtr.h"
#include "P12SaveData.generated.h"

/**
 * Base data that will be saved/loaded.
 */
USTRUCT()
struct FP12BaseSaveData
{
	GENERATED_BODY()

public:
	FP12BaseSaveData();
	virtual ~FP12BaseSaveData() {}
	virtual bool Serialize(FArchive& Archive);

	friend FArchive& operator << (FArchive& Archive, FP12BaseSaveData& SaveData)
	{
		SaveData.Serialize(Archive);
		return Archive;
	}

public:
	FName Name;
};

FORCEINLINE bool operator == (const FP12BaseSaveData& First, const FP12BaseSaveData& Second) { return First.Name == Second.Name; }

/**
 * Object data that will be saved/loaded.
 */
USTRUCT()
struct FP12ObjectSaveData : public FP12BaseSaveData
{
	GENERATED_BODY()

public:
	FP12ObjectSaveData();
	explicit FP12ObjectSaveData(const UObject* Object);
	virtual bool Serialize(FArchive& Archive) override;

public:
	TStrongObjectPtr<UClass> Class;
	TArray<uint8> RawData;
};

/**
 * Actor data that will be saved/loaded.
 */
USTRUCT()
struct FP12ActorSaveData : public FP12ObjectSaveData
{
	GENERATED_BODY()

public:
	FP12ActorSaveData();
	explicit FP12ActorSaveData(const AActor* Actor);
	virtual bool Serialize(FArchive& Archive) override;

public:
	TArray<FP12ObjectSaveData> ObjectsSaveData;
	FTransform Transform;
};

/**
 * Level data that will be saved/loaded.
 */
USTRUCT()
struct FP12LevelSaveData : public FP12ActorSaveData
{
	GENERATED_BODY()

public:
	FP12LevelSaveData();
	explicit FP12LevelSaveData(const FName LevelName);
	virtual bool Serialize(FArchive& Archive) override;

public:
	TArray<FP12ActorSaveData> ActorsSaveData;
};

/**
 * Game sata that will be saved/loaded.
 */
USTRUCT()
struct FP12GameSaveData : public FP12LevelSaveData
{
	GENERATED_BODY()

public:
	FP12GameSaveData();
	virtual bool Serialize(FArchive& Archive) override;

public:
	FName LevelName;
	FP12LevelSaveData Level;
	FP12ObjectSaveData GameInstance;
	FTransform StartTransform;
	bool bSerialized;
};
