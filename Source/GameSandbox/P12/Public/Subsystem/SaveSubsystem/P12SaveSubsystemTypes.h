// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12SaveData.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"
#include "P12SaveSubsystemTypes.generated.h"

class UP12SaveSubsystem;

DECLARE_LOG_CATEGORY_EXTERN(LogP12SaveSubsystem, Log, All)

const FName FileExtensionSave = "save";

/**
 * Used for loading and saving.
 */
struct FP12SaveSubsystemArchive : public FObjectAndNameAsStringProxyArchive
{
public:
	FP12SaveSubsystemArchive(FArchive& InInnerArchive, bool bInLoadIfFindFails);
};

/**
 * Used to get save file names as save id`s.
 */
class FP12SaveDirectoryVisitor : public IPlatformFile::FDirectoryVisitor
{
public:
	explicit FP12SaveDirectoryVisitor(TArray<int32>& InSaveIds);
	virtual bool Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory) override;

private:
	TArray<int32>& SaveIds;
};

/**
 * Used to change bool value for scope.
 */
struct FP12BoolScopeWrapper
{
public:
	FP12BoolScopeWrapper(bool& bInValue, bool bNewValue);
	~FP12BoolScopeWrapper();

private:
	bool& bValue;
	bool bInitialValue;
};

/**
 * UP12StreamingLevelObserver
 */

UCLASS()
class UP12StreamingLevelObserver : public UObject
{
	GENERATED_BODY()

public:
	UP12StreamingLevelObserver();
	void Initialize(UP12SaveSubsystem* InSaveSubsystem, ULevelStreaming* InStreamingLevel);
	void Deinitialize();
	virtual void Serialize(FArchive& Archive) override;

private:
	UFUNCTION()
	void OnLevelShown();
	UFUNCTION()
	void OnLevelHidden();

private:
	FP12LevelSaveData LevelSaveData;
	TWeakObjectPtr<UP12SaveSubsystem> SaveSubsystem;
	TWeakObjectPtr<ULevelStreaming> StreamingLevel;
};