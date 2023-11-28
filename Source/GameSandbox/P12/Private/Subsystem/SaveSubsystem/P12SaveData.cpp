#include "P12/Public/Subsystem/SaveSubsystem/P12SaveData.h"

/**
 * FP12BaseSaveData
 */

FP12BaseSaveData::FP12BaseSaveData()
{
		
}

bool FP12BaseSaveData::Serialize(FArchive& Archive)
{
	Archive << Name;
	return true;
}

FP12ObjectSaveData::FP12ObjectSaveData()
	:Class(nullptr)
{
	
}

/**
 * FP12ObjectSaveData
 */

FP12ObjectSaveData::FP12ObjectSaveData(const UObject* Object)
	:Class(nullptr)
{
	if (!Object)
	{
		return;
	}
	Name = *Object->GetName();
	Class = TStrongObjectPtr<UClass>(Object->GetClass());
}

bool FP12ObjectSaveData::Serialize(FArchive& Archive)
{
	FP12BaseSaveData::Serialize(Archive);

	UClass* ClassPtr = Class.Get();
	Archive << ClassPtr;

	if (Archive.IsLoading() && ClassPtr)
	{
		Class = TStrongObjectPtr<UClass>(ClassPtr);
	}
	Archive << RawData;
	return true;
}

/**
 * FP12ActorSaveData
 */

FP12ActorSaveData::FP12ActorSaveData()
	:Transform(FTransform::Identity)
{
	
}

FP12ActorSaveData::FP12ActorSaveData(const AActor* Actor)
	:Super(Actor), Transform(FTransform::Identity)
{
	
}

bool FP12ActorSaveData::Serialize(FArchive& Archive)
{
	FP12ObjectSaveData::Serialize(Archive);

	Archive << Transform;
	Archive << ObjectsSaveData;
	return true;
}

/**
 * FP12LevelSaveData
 */

FP12LevelSaveData::FP12LevelSaveData()
{
	
}

FP12LevelSaveData::FP12LevelSaveData(const FName LevelName)
{
	Name = LevelName;
}

bool FP12LevelSaveData::Serialize(FArchive& Archive)
{
	FP12ActorSaveData::Serialize(Archive);

	Archive << ActorsSaveData;
	return true;
}

/**
 * FP12GameSaveData
 */

FP12GameSaveData::FP12GameSaveData()
	:PersistentLevel("Persistent"), StartTransform(FTransform::Identity), bSerialized(false)
{
	
}

bool FP12GameSaveData::Serialize(FArchive& Archive)
{
	FP12LevelSaveData::Serialize(Archive);

	bSerialized = true;

	Archive << LevelName;
	Archive << PersistentLevel;
	Archive << StreamingLevels;
	Archive << GameInstance;
	Archive << StartTransform;
	Archive << bSerialized;
	
	return true;
}
