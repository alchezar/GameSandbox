// Copyright (C) 2023, IKinder

#include "P12/Public/Util/P12ReadWriteFile.h"

#include "AssetImportTask.h"
#include "AssetToolsModule.h"
#include "JsonObjectConverter.h"
#include "Factories/ReimportDataTableFactory.h"

/* ----------------------------- Text file ----------------------------- */

FString UP12ReadWriteFile::ReadStringFromFile(const FString& Filepath, FP12ReturnInfo& ReturnInfo)
{
	FString Result = "";

	/* Check if the file exists. */
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*Filepath))
	{
		ReturnInfo = {false, "Read string from file failed - File does`n exist - " + Filepath};
		return Result;
	}
	/* Try to read the file. Output goes in Result string. */
	if (!FFileHelper::LoadFileToString(Result, *Filepath))
	{
		ReturnInfo = {false, "Read string from file failed - Was not able to read the file. Is this a text file? - " + Filepath};
		return Result;
	}
	ReturnInfo = {true, "Read string from file succeeded - " + Filepath};
	return Result;
}

void UP12ReadWriteFile::WriteStringToFile(const FString& Filepath, const FString String, FP12ReturnInfo& ReturnInfo)
{
	/* Try to write the string into the file. */
	if (!FFileHelper::SaveStringToFile(String, *Filepath))
	{
		ReturnInfo = {false, "Write string to file failed - Wasn't able to write into file. Is your file read only? Is the path valid? - " + Filepath};
	}
	ReturnInfo = {true, "Write string to file succeeded - " + Filepath};
}

/* ----------------------------- Json file ----------------------------- */

TSharedPtr<FJsonObject> UP12ReadWriteFile::ReadJson(const FString& JsonFilepath, FP12ReturnInfo& ReturnInfo)
{
	TSharedPtr<FJsonObject> Result;

	/* Try to read the file. Output goes in Result shared pointer. */
	const FString JsonString = ReadStringFromFile(JsonFilepath, ReturnInfo);
	if (!ReturnInfo.bOutSuccess)
	{
		return Result;
	}
	/* Try to convert the string into the json object. Output goes in Result shared pointer. */
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), Result))
	{
		ReturnInfo = {false, "Read Json failed - Was not able to deserialize the json string. Is it the right format? - " + JsonFilepath};
		return Result;
	}
	ReturnInfo = {true, "Read Json succeeded - " + JsonFilepath};
	return Result;
}

void UP12ReadWriteFile::WriteJson(const FString& JsonFilepath, const TSharedPtr<FJsonObject>& JsonObject, FP12ReturnInfo& ReturnInfo)
{
	FString JsonString;
	/* Try to convert json object into the string. Output goes in JsonString. */
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString)))
	{
		ReturnInfo = {false, "Write json failed - Was not able to serialize the json to the string. Is the JsonObject valid? - " + JsonFilepath};
		return;
	}
	/* Try to write json string to file. */
	WriteStringToFile(JsonFilepath, JsonString, ReturnInfo);
	if (!ReturnInfo.bOutSuccess)
	{
		return;
	}
	ReturnInfo = {true, "Write Json succeeded - " + JsonFilepath};
}

/* ---------------------------- Struct file ---------------------------- */

FP12JsonTestStruct UP12ReadWriteFile::ReadTestStructFromJsonFile(const FString& JsonFilepath, FP12ReturnInfo& ReturnInfo)
{
	FP12JsonTestStruct Result;

	/* Try to read generic json object from file. */
	const TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilepath, ReturnInfo);
	if (!ReturnInfo.bOutSuccess)
	{
		return FP12JsonTestStruct();
	}
	/* Try to convert generic json object into the desired structure. Output goes in Result structure. */
	if (!FJsonObjectConverter::JsonObjectToUStruct<FP12JsonTestStruct>(JsonObject.ToSharedRef(), &Result))
	{
		ReturnInfo = {false, "Read struct json failed – Was not able to convert the json object to your desired structure. Is it the right format / struct? - " + JsonFilepath};
		return FP12JsonTestStruct();
	}
	ReturnInfo = {true, "Read struct json succeeded - " + JsonFilepath};
	return Result;
}

void UP12ReadWriteFile::WriteTestStructToJsonFile(const FString& JsonFilepath, const FP12JsonTestStruct& Struct, FP12ReturnInfo& ReturnInfo)
{
	/* Try to convert the struct to generic json object. */
	const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
	if (!JsonObject.IsValid())
	{
		ReturnInfo = {false, "Write struct json failed - Was not able to convert the struct to a json object. This shouldn't really happen - " + JsonFilepath};
		return;
	}
	/* Try to write json to file. */
	WriteJson(JsonFilepath, JsonObject, ReturnInfo);
}

/* --------------------------- Import asset ---------------------------- */

UAssetImportTask* UP12ReadWriteFile::CreateImportTask(FString SourcePath, const FString& DestinationPath, UFactory* ExtraFactory, UObject* ExtraOptions, FP12ReturnInfo& ReturnInfo)
{
	/* Create task object. */
	UAssetImportTask* ImportTask = NewObject<UAssetImportTask>();

	/* Make sure the object was created properly. */
	if (!ImportTask)
	{
		ReturnInfo = {false, "Create import task failed - Was not able to create the import task object. That shouldn't really happen - " + SourcePath};
		return nullptr;
	}
	/* Set path information. */
	ImportTask->Filename = SourcePath;
	ImportTask->DestinationPath = FPaths::GetPath(DestinationPath);
	ImportTask->DestinationName = FPaths::GetCleanFilename(DestinationPath);

	/* Set basic options. */
	ImportTask->bSave = false;
	ImportTask->bAutomated = true;
	ImportTask->bAsync = false;
	ImportTask->bReplaceExisting = true;
	ImportTask->bReplaceExistingSettings = false;

	/* Optional extra factory and options. */
	ImportTask->Factory = ExtraFactory;
	ImportTask->Options = ExtraOptions;

	/* Return the task. */
	ReturnInfo = {true, "Create import task succeeded - " + SourcePath};
	return ImportTask;
}

UObject* UP12ReadWriteFile::ProcessImportTask(UAssetImportTask* ImportTask, FP12ReturnInfo& ReturnInfo)
{
	/* Make sure the import task is valid. */
	if (!ImportTask)
	{
		ReturnInfo = {false, "Process import task failed - The task was invalid. Cannot process - " + ImportTask->Filename};
		return nullptr;
	}

	/* Get asset tool module. */
	const FAssetToolsModule* AssetTools = FModuleManager::LoadModulePtr<FAssetToolsModule>("AssetTools");
	if (!AssetTools)
	{
		ReturnInfo = {false, "Process import task failed - The asset tool module is invalid - " + ImportTask->Filename};
		return nullptr;
	}

	/* Import the asset. */
	AssetTools->Get().ImportAssetTasks({ImportTask});

	/* Check if anything was imported during the process. */
	if (ImportTask->GetObjects().IsEmpty())
	{
		ReturnInfo = {false, "Process import task failed - Nothing was imported. Is your file valid? Is the asset type supported? - " + ImportTask->Filename};
		return nullptr;
	}

	/* Because some import tasks actually creates multiple assets (e.g. SkeletalMesh), we manually get the right asset. */
	UObject* ImportedAsset = StaticLoadObject(UObject::StaticClass(), nullptr, *FPaths::Combine(ImportTask->DestinationPath, ImportTask->DestinationName));

	/* Return the asset. */
	ReturnInfo = {true, "Process import task succeeded - " + ImportTask->Filename};
	return ImportedAsset;
}

UObject* UP12ReadWriteFile::ImportAsset(FString SourcePath, FString DestinationPath, FP12ReturnInfo& ReturnInfo)
{
	/* Create import task. */
	UAssetImportTask* ImportTask = CreateImportTask(SourcePath, DestinationPath, nullptr, nullptr, ReturnInfo);
	if (!ReturnInfo.bOutSuccess)
	{
		return nullptr;
	}

	/* Import the asset. */
	UObject* ReturnAsset = ProcessImportTask(ImportTask, ReturnInfo);
	if (!ReturnInfo.bOutSuccess)
	{
		return nullptr;
	}

	/* Return the imported asset. */
	ReturnInfo = {true, "Import asset succeeded - " + DestinationPath};
	return ReturnAsset;
}

/* ---------------------------- Data table ----------------------------- */

UDataTable* UP12ReadWriteFile::ImportDataTableFromJsonOrCsv(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass, FP12ReturnInfo& ReturnInfo)
{
	/* Create factory to import a data table. */
	UReimportDataTableFactory* DataTableFactory = NewObject<UReimportDataTableFactory>();
	DataTableFactory->AutomatedImportSettings.ImportType = ECSVImportType::ECSV_DataTable;
	DataTableFactory->AutomatedImportSettings.ImportRowStruct = StructClass;

	/* Create import task. */
	UAssetImportTask* ImportTask = CreateImportTask(SourcePath, DestinationPath, DataTableFactory, nullptr, ReturnInfo);
	if (!ReturnInfo.bOutSuccess)
	{
		return nullptr;
	}

	/* Import asset. */
	UObject* ImportedAsset = ProcessImportTask(ImportTask, ReturnInfo);
	if (!ReturnInfo.bOutSuccess)
	{
		return nullptr;
	}

	/* Return imported asset. */
	ReturnInfo = {true, "Import data table succeeded - " + DestinationPath};
	return Cast<UDataTable>(ImportedAsset);
}

void UP12ReadWriteFile::ExportDataTableToJsonOrCsv(FString FilePath, UDataTable* DataTable, FP12ReturnInfo& ReturnInfo)
{
	/* Check if data table is valid. */
	if (!DataTable)
	{
		ReturnInfo = {false, "Export data table to json or csv failed - Data table is nullptr. Filepath - " + FilePath};

		return;
	}

	/* Convert table to string. */
	FString TableString;

	if (FilePath.Contains(".csv"))
	{
		TableString = DataTable->GetTableAsCSV();
	}
	else
	{
		TableString = DataTable->GetTableAsJSON();
	}

	/* Write string to file. */
	WriteStringToFile(FilePath, TableString, ReturnInfo);
}

/* ------------------------- Data table Json --------------------------- */

TMap<FString, FP12JsonTestStruct> UP12ReadWriteFile::ReadStructFromJsonFile_DataTableFormat(FString FilePath, FP12ReturnInfo& ReturnInfo)
{
	/* Read file. */
	const FString JsonString = ReadStringFromFile(FilePath, ReturnInfo);
	if (!ReturnInfo.bOutSuccess)
	{
		return TMap<FString, FP12JsonTestStruct>();
	}

	/* Create data table and tell it which struct it's using. */
	UDataTable* Table = NewObject<UDataTable>();
	Table->RowStruct = FP12JsonTestStruct::StaticStruct();

	/* Populate data table. */
	Table->CreateTableFromJSONString(JsonString);

	/* Retrieve the rows. */
	TArray<FName> RowNames = Table->GetRowNames();

	/* Populate the return map. */
	TMap<FString, FP12JsonTestStruct> RowsToStruct;

	for (FName RowName : RowNames)
	{
		const FP12JsonTestStruct* Row = Table->FindRow<FP12JsonTestStruct>(RowName, nullptr);
		if (!Row)
		{
			continue;
		}
		RowsToStruct.Add(RowName.ToString(), *Row);
	}

	/* Return the rows. */
	ReturnInfo = {true, "Read data table json succeeded - " + FilePath};
	return RowsToStruct;
}

void UP12ReadWriteFile::WriteStructToJsonFile_DataTableFormat(FString FilePath, TMap<FString, FP12JsonTestStruct> RowsToStruct, FP12ReturnInfo& ReturnInfo)
{
	/* Convert all rows to string. */
	TArray<FString> RowNames;
	RowsToStruct.GetKeys(RowNames);

	/* Create data table and tell it which struct it's using. */
	UDataTable* Table = NewObject<UDataTable>();
	Table->RowStruct = FP12JsonTestStruct::StaticStruct();

	/* Add all rows to data table. */
	for (FString RowName : RowNames)
	{
		Table->AddRow(*RowName, RowsToStruct[RowName]);
	}

	/* Export to file. */
	ExportDataTableToJsonOrCsv(FilePath, Table, ReturnInfo);
}
