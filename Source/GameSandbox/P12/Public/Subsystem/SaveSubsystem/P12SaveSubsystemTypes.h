// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Serialization/ObjectAndNameAsStringProxyArchive.h"

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