#include "Subsystem/SaveSubsystem/P12SaveSubsystemTypes.h"

#include "Subsystem/SaveSubsystem/P12SaveSubsystem.h"

DEFINE_LOG_CATEGORY(LogP12SaveSubsystem)

/**
 * FP12SaveSubsystemArchive
 */

FP12SaveSubsystemArchive::FP12SaveSubsystemArchive(FArchive& InInnerArchive, bool bInLoadIfFindFails)
	: FObjectAndNameAsStringProxyArchive(InInnerArchive, bInLoadIfFindFails)
{
	ArIsSaveGame = true;
	ArNoDelta = true;
}

/**
 * FP12SaveDirectoryVisitor
 */

FP12SaveDirectoryVisitor::FP12SaveDirectoryVisitor(TArray<int32>& InSaveIds)
	: SaveIds(InSaveIds)
{
	//
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

/**
 * UP12StreamingLevelObserver
 */

UP12StreamingLevelObserver::UP12StreamingLevelObserver()
{
	//
}

void UP12StreamingLevelObserver::Initialize(UP12SaveSubsystem* InSaveSubsystem, ULevelStreaming* InStreamingLevel)
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12StreamingLevelObserver::Initialize(): StreamingLevel %s"), *GetNameSafe(InStreamingLevel));

	if (!InSaveSubsystem || !InStreamingLevel)
	{
		return;
	}
	SaveSubsystem = InSaveSubsystem;
	StreamingLevel = InStreamingLevel;

	StreamingLevel->OnLevelShown.AddUniqueDynamic(this, &ThisClass::OnLevelShown);
	StreamingLevel->OnLevelHidden.AddUniqueDynamic(this, &ThisClass::OnLevelHidden);
}

void UP12StreamingLevelObserver::Deinitialize()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12StreamingLevelObserver::Initialize(): StreamingLevel %s"), *GetNameSafe(StreamingLevel.Get()));

	if (StreamingLevel.IsValid())
	{
		StreamingLevel->OnLevelShown.RemoveDynamic(this, &ThisClass::OnLevelShown);
		StreamingLevel->OnLevelHidden.RemoveDynamic(this, &ThisClass::OnLevelHidden);
	}
	StreamingLevel.Reset();
	SaveSubsystem.Reset();
}

void UP12StreamingLevelObserver::Serialize(FArchive& Archive)
{
	// UObject::Serialize(Ar);
	LevelSaveData.Serialize(Archive);
}

void UP12StreamingLevelObserver::OnLevelShown()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12StreamingLevelObserver::OnLevelShown(): StreamingLevel %s"), *GetNameSafe(StreamingLevel.Get()));

	if (!SaveSubsystem.IsValid() || !StreamingLevel.IsValid())
	{
		return;
	}
	SaveSubsystem->DeserializeLevel(StreamingLevel->GetLoadedLevel(), StreamingLevel.Get());
}

void UP12StreamingLevelObserver::OnLevelHidden()
{
	UE_LOG(LogP12SaveSubsystem, Display, TEXT("UP12StreamingLevelObserver::OnLevelHidden(): StreamingLevel %s"), *GetNameSafe(StreamingLevel.Get()));

	if (!SaveSubsystem.IsValid() || !StreamingLevel.IsValid())
	{
		return;
	}
	SaveSubsystem->SerializeLevel(StreamingLevel->GetLoadedLevel(), StreamingLevel.Get());
}
