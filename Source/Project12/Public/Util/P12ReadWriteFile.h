// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P12ReadWriteFile.generated.h"

USTRUCT(BlueprintType, Category = "C++ | Kinder")
struct FP12ReturnInfo
{
	GENERATED_BODY()

public:
	FP12ReturnInfo()
		: bOutSuccess(false), OutInfoMessage("") {};

	/**
	 * Return info constructor
	 *
	 * @param bNewSuccess		If the action was successful or not.
	 * @param NewInfoMessage	More information about action`s result.
	 */
	FP12ReturnInfo(const bool bNewSuccess, const FString& NewInfoMessage)
		: bOutSuccess(bNewSuccess), OutInfoMessage(NewInfoMessage) {};

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	bool bOutSuccess;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString OutInfoMessage;
};

USTRUCT(BlueprintType, Category = "C++ | Kinder")
struct FP12JsonTestStruct : public FTableRowBase
{
	GENERATED_BODY()

public:
	/**
	 * These variables are visible in both Blueprint and is in Json.
	 */
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString MyString = "Llama";
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool MyBool = true;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MyFloat = 123.456f;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int MyInt = 123;

	/**
	 * These variables are not visible in Blueprint, but are still visible in Json.
	 */
	UPROPERTY()
	FVector MyVector = {1.0f, 2.0f, 3.0f};
	UPROPERTY()
	FRotator MyRotator = {90.0f, 180.0f, 270.0f};
	UPROPERTY()
	FTransform MyTransform;

	/*
	 * This variable is not visible in Blueprint or json.
	 */
	FString MyOtherString = "This variable will not be in the json because it's not flagged as an UPROPERTY()";
};

UCLASS()
class PROJECT12_API UP12ReadWriteFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* ----------------------------- Text file ----------------------------- */

	/**
	 * Open a text file and read its content.
	 *
	 * @param Filepath		    The path of your file.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The string we wanted to read.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static FString ReadStringFromFile(const FString& Filepath, FP12ReturnInfo& ReturnInfo);

	/**
	 * Create a text file with your content.
	 *
	 * @param Filepath			The path of your file.
	 * @param String			The string to write in the file.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteStringToFile(const FString& Filepath, const FString String, FP12ReturnInfo& ReturnInfo);

	/* ----------------------------- Json file ----------------------------- */

	/**
	 * Open a json file. Read it`s content and convert into a JsonObject.
	 *
	 * @param JsonFilepath		The path of your .json file.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The JsonObject content of your json file.
	 */
	// UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static TSharedPtr<FJsonObject> ReadJson(const FString& JsonFilepath, FP12ReturnInfo& ReturnInfo);

	/**
	 * Convert your JsonObject to a string, create .json file and dump the string in it.
	 *
	 * @param JsonFilepath		The path of your .json file.
	 * @param JsonObject		The json object to write in the file.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 */
	// UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteJson(const FString& JsonFilepath, const TSharedPtr<FJsonObject>& JsonObject, FP12ReturnInfo& ReturnInfo);

	/* ---------------------------- Struct file ---------------------------- */

	/**
	 * Open a json file, read its content and dump it into a structure.
	 * 
	 * @param JsonFilepath		The path of your json file: "C:/Temp/MyJson.json".
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The structure content of your json file.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static FP12JsonTestStruct ReadTestStructFromJsonFile(const FString& JsonFilepath, FP12ReturnInfo& ReturnInfo);

	/**
	 * Convert your structure to a string, create a json file and dump the string in it.
	 *
	 * @param JsonFilepath		The path of your json file: "C:/Temp/MyJson.json".
	 * @param Struct			The struct to want to write in the file.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteTestStructToJsonFile(const FString& JsonFilepath, const FP12JsonTestStruct& Struct, FP12ReturnInfo& ReturnInfo);

	/* --------------------- Template Struct Json file --------------------- */

	/**
	 * Open a json file, read its content and dump it into a structure.
	 * 
	 * @param JsonFilepath		The path of your json file: "C:/Temp/MyJson.json".
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The structure content of your json file.
	 */
	template <typename T>
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static T ReadStructFromJsonFile(const FString& JsonFilepath, FP12ReturnInfo& ReturnInfo)
	{
		T Result;

		/* Try to read generic json object from file. */
		const TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilepath, ReturnInfo);
		if (!ReturnInfo.bOutSuccess)
		{
			return T();
		}
		/* Try to convert generic json object into the desired structure. Output goes in Result structure. */
		if (!FJsonObjectConverter::JsonObjectToUStruct<T>(JsonObject.ToSharedRef(), &Result))
		{
			ReturnInfo = {false, "Read struct json failed – Was not able to convert the json object to your desired structure. Is it the right format / struct? - " + JsonFilepath};
			return T();
		}
		ReturnInfo = {true, "Read struct json succeeded - " + JsonFilepath};
		return Result;
	}

	/**
	 * Convert your structure to a string, create a json file and dump the string in it.
	 *
	 * @param JsonFilepath		The path of your json file: "C:/Temp/MyJson.json".
	 * @param Struct			The struct to want to write in the file.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 */
	template <typename T>
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteStructToJsonFile(const FString& JsonFilepath, const T& Struct, FP12ReturnInfo& ReturnInfo)
	{
		/* Try to convert the struct to generic json object. */
		const TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(Struct);
		if (!JsonObject.IsValid())
		{
			ReturnInfo = {false, "Write struct json failed - Was not able to convert the struct to a json object. This shouldn't really happen. - " + JsonFilepath};
			return;
		}
		/* Try to write json to file. */
		WriteJson(JsonFilepath, JsonObject, ReturnInfo);
	}

	/* --------------------------- Import asset ---------------------------- */

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Create an import task that you can then process to import any kind of assets
	 *
	 * @param SourcePath		The path of the source file: "C:/Temp//Texture.peg"
	 * @param DestinationPath	The path of the Imported asset: /Game/Folder/MyTexture"
	 * @param ExtraFactory		Optional. Some asset types require a special factory to import them
	 * @param ExtraOptions		Optional. Some asset types have some extra settings you can set
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The import task
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static UAssetImportTask* CreateImportTask(FString SourcePath, const FString& DestinationPath, UFactory* ExtraFactory, UObject* ExtraOptions, FP12ReturnInfo& ReturnInfo);

	/**
	 * Editor Daly - Will not work in packaged build.
	 *
	 * Process the import task to import the assets
	 *
	 * @param ImportTask		The task you want to process
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The Imported asset
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static UObject* ProcessImportTask(UAssetImportTask* ImportTask, FP12ReturnInfo& ReturnInfo);

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Create and process a basic import task.
	 * Will work for the basic asset types like textures and sounds:
	 * Can also be used for meshes, but you might not have all the control you want.
	 *
	 * @param SourcePath		The path of the source file: "C:/Temp//Texture.peg"
	 * @param DestinationPath	The path of the Imported asset: /Game/Folder/MyTexture"
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The Imported asset
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static UObject* ImportAsset(FString SourcePath, FString DestinationPath, FP12ReturnInfo& ReturnInfo);

	/* ---------------------------- Data table ----------------------------- */

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Import a data table from a json or csv file.
	 *
	 * @param SourcePath		The path of the source file: "C:/Temp/My3son.json".
	 * @param DestinationPath	The path of the imported asset: "/Game/Folder/MyDataTable".
	 * @param StructClass		The structure to use for this data table.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The Imported data table.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static UDataTable* ImportDataTableFromJsonOrCsv(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass, FP12ReturnInfo& ReturnInfo);

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Export a data table to a json or csv file.
	 * 
	 * @param FilePath			The path of the output file: "C:/Temp/y3son.json".
	 * @param DataTable			The data table to sport.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void ExportDataTableToJsonOrCsv(FString FilePath, UDataTable* DataTable, FP12ReturnInfo& ReturnInfo);

	/* ------------------------- Data table Json --------------------------- */

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Read a series of struct based on a json file formatted like a data table.
	 *
	 * @param FilePath			The path of the source file: "C:/Temp/MyJson.json"
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 * @return					The structs
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static TMap<FString, FP12JsonTestStruct> ReadStructFromJsonFile_DataTableFormat(FString FilePath, FP12ReturnInfo& ReturnInfo);

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Write a series of struct to a json file. Will be formatted like a data table.
	 *
	 * @param FilePath			The path of the output file: "C:/Temp/My3son.json".
	 * @param RowsToStruct		The structs to write in the json.
	 * @param ReturnInfo		If the action was successful or not and more information about action`s result.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteStructToJsonFile_DataTableFormat(FString FilePath, TMap<FString, FP12JsonTestStruct> RowsToStruct, FP12ReturnInfo& ReturnInfo);
};
