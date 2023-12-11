// Copyright (C) 2023, IKinder

#include "P12/Public/Util/P12ReadWriteFile.h"

#include "JsonObjectConverter.h"

/* ----------------------------- Text file ----------------------------- */

FString UP12ReadWriteFile::ReadStringFromFile(const FString& Filepath, bool& bOutSuccess, FString& OutInfoMessage)
{
	FString Result = "";

	/* Check if the file exists. */
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*Filepath))
	{
		bOutSuccess = false;
		OutInfoMessage = "Read string from file failed - File does`n exist - " + Filepath;
		return Result;
	}
	/* Try to read the file. Output goes in Result string. */
	if (!FFileHelper::LoadFileToString(Result, *Filepath))
	{
		bOutSuccess = false;
		OutInfoMessage = "Read string from file failed - Was not able to read the file. Is this a text file? - " + Filepath;
		return Result;
	}
	bOutSuccess = true;
	OutInfoMessage = "Read string from file succeeded - " + Filepath;
	return Result;
}

void UP12ReadWriteFile::WriteStringToFile(const FString& Filepath, const FString String, bool& bOutSuccess, FString& OutInfoMessage)
{
	/* Try to write the string into the file. */
	if (!FFileHelper::SaveStringToFile(String, *Filepath))
	{
		bOutSuccess = false;
		OutInfoMessage = "Write string to file failed - Wasn't able to write into file. Is your file read only? Is the path valid? - " + Filepath;
	}
	bOutSuccess = true;
	OutInfoMessage = "Write string to file succeeded - " + Filepath;
}

/* ----------------------------- Json file ----------------------------- */

TSharedPtr<FJsonObject> UP12ReadWriteFile::ReadJson(const FString& JsonFilepath, bool& bOutSuccess, FString& OutInfoMessage)
{
	TSharedPtr<FJsonObject> Result;

	/* Try to read the file. Output goes in Result shared pointer. */
	const FString JsonString = ReadStringFromFile(JsonFilepath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return Result;
	}
	/* Try to convert the string into the json object. Output goes in Result shared pointer. */
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(JsonString), Result))
	{
		bOutSuccess = false;
		OutInfoMessage = "Read Json failed - Was not able to deserialize the json string. Is it the right format? - " + JsonFilepath;
		return Result;
	}
	bOutSuccess = true;
	OutInfoMessage = "Read Json succeeded - " + JsonFilepath;
	return Result;
}

void UP12ReadWriteFile::WriteJson(const FString& JsonFilepath, const TSharedPtr<FJsonObject>& JsonObject, bool& bOutSuccess, FString& OutInfoMessage)
{
	FString JsonString;
	/* Try to convert json object into the string. Output goes in JsonString. */
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString)))
	{
		bOutSuccess = false;
		OutInfoMessage = "Write json failed - Was not able to serialize the json to the string. Is the JsonObject valid? - " + JsonFilepath;
		return;
	}
	/* Try to write json string to file. */
	WriteStringToFile(JsonFilepath, JsonString, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return;
	}
	bOutSuccess = true;
	OutInfoMessage = "Write Json succeeded - " + JsonFilepath;
}

/* ---------------------------- Struct file ---------------------------- */

FP12JsonTestStruct UP12ReadWriteFile::ReadTestStructFromJsonFile(const FString& JsonFilepath, bool& bOutSuccess, FString& OutInfoMessage)
{
	FP12JsonTestStruct Result;

	/* Try to read generic json object from file. */
	const TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilepath, bOutSuccess, OutInfoMessage);
	if (!bOutSuccess)
	{
		return FP12JsonTestStruct();
	}
	/* Try to convert generic json object into the desired structure. Output goes in Result structure. */
	if (!FJsonObjectConverter::JsonObjectToUStruct<FP12JsonTestStruct>(JsonObject.ToSharedRef(), &Result))
	{
		bOutSuccess = false;
		OutInfoMessage = "Read struct json failed – Was not able to convert the json object to your desired structure. Is it the right format / struct? - " + JsonFilepath;
		return FP12JsonTestStruct();
	}
	bOutSuccess = true;
	OutInfoMessage = "Read struct json succeeded - " + JsonFilepath;
	return Result;
}

void UP12ReadWriteFile::WriteTestStructToJsonFile(const FString& JsonFilepath, const FP12JsonTestStruct& Struct, bool& bOutSuccess, FString& OutInfoMessage)
{
	/* Try to convert the struct to generic json object. */
	const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
	if (!JsonObject.IsValid())
	{
		bOutSuccess = false;
		OutInfoMessage = "Write struct json failed - Was not able to convert the struct to a json object. This shouldn't really happen.";
		return;
	}
	/* Try to write json to file. */
	WriteJson(JsonFilepath, JsonObject, bOutSuccess, OutInfoMessage);
}
