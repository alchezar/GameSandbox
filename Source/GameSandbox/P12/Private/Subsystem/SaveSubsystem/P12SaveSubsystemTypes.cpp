#include "P12/Public/Subsystem/SaveSubsystem/P12SaveSubsystemTypes.h"

DEFINE_LOG_CATEGORY(LogP12SaveSubsystem)

/**
 * FP12SaveSubsystemArchive
 */

FP12SaveSubsystemArchive::FP12SaveSubsystemArchive(FArchive& InInnerArchive, bool bInLoadIfFindFails)
	:FObjectAndNameAsStringProxyArchive(InInnerArchive, bInLoadIfFindFails)
{
	ArIsSaveGame = true;
	ArNoDelta = true;
}

/**
 * FP12SaveDirectoryVisitor
 */

FP12SaveDirectoryVisitor::FP12SaveDirectoryVisitor(TArray<int32>& InSaveIds)
	:SaveIds(InSaveIds)
{
	
}

bool FP12SaveDirectoryVisitor::Visit(const TCHAR* FilenameOrDirectory, bool bIsDirectory)
{
	if (bIsDirectory)
	{
		return true;
	}
	const FString FullFilePath = FilenameOrDirectory;

	FString DirectoryName;
	FString Filename;
	FString FileExtension;
	FPaths::Split(FullFilePath, DirectoryName, Filename, FileExtension);

	if (FileExtension == FileExtensionSave.ToString())
	{
		const int32 SaveId = FCString::Atoi(*Filename);
		if (SaveId > 0)
		{
			SaveIds.Add(SaveId);
		}
	}
	return true;
}

/**
 * FP12BoolScopeWrapper
 */

FP12BoolScopeWrapper::FP12BoolScopeWrapper(bool& bInValue, bool bNewValue)
	: bValue(bInValue), bInitialValue(bInValue)
{
	bValue = bNewValue;
}

FP12BoolScopeWrapper::~FP12BoolScopeWrapper()
{
	bValue = bInitialValue;
}
