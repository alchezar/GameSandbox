// Copyright © 2025, Ivan Kinder

#include "Utils/P14JsonUtils.h"

#include "JsonObjectConverter.h"

DEFINE_LOG_CATEGORY_STATIC(LogP14JsonUtils, All, All)

using namespace P14::Test;

bool FJsonUtils::WriteInputData(const FString& FileName, const FP14InputData& InputData)
{
	const TSharedPtr<FJsonObject> MainJsonObject = FJsonObjectConverter::UStructToJsonObject(InputData);
	if (!MainJsonObject.IsValid())
	{
		UE_LOG(LogP14JsonUtils, Error, L"Failed to convert input data to json");
		return false;
	}

	FString                         OutputString = "";
	const TSharedRef<TJsonWriter<>> Writer       = TJsonWriterFactory<>::Create(&OutputString);

	if (!FJsonSerializer::Serialize(MainJsonObject.ToSharedRef(), Writer))
	{
		UE_LOG(LogP14JsonUtils, Error, L"Failed to serialize input data");
		return false;
	}
	Writer->Close();

	if (!FFileHelper::SaveStringToFile(OutputString, *FileName))
	{
		UE_LOG(LogP14JsonUtils, Error, L"Failed to save file %s", *FileName);
		return false;
	}

	return true;
}

bool FJsonUtils::ReadInputData(const FString& FileName, FP14InputData& InputData)
{
	TSharedPtr<FJsonObject> MainJsonObject = MakeShareable(new FJsonObject{});

	FString FileContentString = "";
	if (!FFileHelper::LoadFileToString(FileContentString, *FileName))
	{
		UE_LOG(LogP14JsonUtils, Error, L"Failed to load file %s", *FileName);
		return false;
	}

	const TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(FileContentString);
	if (!FJsonSerializer::Deserialize(Reader, MainJsonObject))
	{
		UE_LOG(LogP14JsonUtils, Error, L"Failed to deserialize input data");
		return false;
	}

	return FJsonObjectConverter::JsonObjectToUStruct(MainJsonObject.ToSharedRef(), &InputData);
}
