// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "JsonObjectConverter.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P12ReadWriteFile.generated.h"

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
class GAMESANDBOX_API UP12ReadWriteFile : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/* ----------------------------- Text file ----------------------------- */

	/**
	 * Open a text file and read its content.
	 *
	 * @param Filepath		    The path of your file.
	 * @param bOutSuccess		If the action was successful or not.
	 * @param OutInfoMessage	More information about action`s result.
	 * @return					The string we wanted to read.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static FString ReadStringFromFile(const FString& Filepath, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Create a text file with your content.
	 *
	 * @param Filepath			The path of your file.
	 * @param String			The string to write in the file.
	 * @param bOutSuccess	    If the action was successful or not.
	 * @param OutInfoMessage	More information about action`s result.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteStringToFile(const FString& Filepath, const FString String, bool& bOutSuccess, FString& OutInfoMessage);

	/* ----------------------------- Json file ----------------------------- */

	/**
	 * Open a json file. Read it`s content and convert into a JsonObject.
	 *
	 * @param JsonFilepath		The path of your .json file.
	 * @param bOutSuccess		If the action was successful or not.
	 * @param OutInfoMessage	More information about action`s result.
	 * @return					The JsonObject content of your json file.
	 */
	// UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static TSharedPtr<FJsonObject> ReadJson(const FString& JsonFilepath, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Convert your JsonObject to a string, create .json file and dump the string in it.
	 *
	 * @param JsonFilepath		The path of your .json file.
	 * @param JsonObject		The json object to write in the file.
	 * @param bOutSuccess		If the action was successful or not.
	 * @param OutInfoMessage	More information about action`s result.
	 */
	// UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteJson(const FString& JsonFilepath, const TSharedPtr<FJsonObject>& JsonObject, bool& bOutSuccess, FString& OutInfoMessage);

	/* ---------------------------- Struct file ---------------------------- */

	/**
	 * Open a json file, read its content and dump it into a structure.
	 * 
	 * @param JsonFilepath		The path of your json file: "C:/Temp/MyJson.json".
	 * @param bOutSuccess		If the action was a success or not.
	 * @param OutInfoMessage	More information about the action's result.
	 * @return					The structure content of your json file.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static FP12JsonTestStruct ReadTestStructFromJsonFile(const FString& JsonFilepath, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Convert your structure to a string, create a json file and dump the string in it.
	 *
	 * @param JsonFilepath		The path of your json file: "C:/Temp/MyJson.json".
	 * @param Struct			The struct to want to write in the file.
	 * @param bOutSuccess		If the action was a success or not.
	 * @param OutInfoMessage	More information about the action's result
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteTestStructToJsonFile(const FString& JsonFilepath, const FP12JsonTestStruct& Struct, bool& bOutSuccess, FString& OutInfoMessage);

	/* --------------------- Template Struct Json file --------------------- */

	/**
	 * Open a json file, read its content and dump it into a structure.
	 * 
	 * @param JsonFilepath		The path of your json file: "C:/Temp/MyJson.json".
	 * @param bOutSuccess		If the action was a success or not.
	 * @param OutInfoMessage	More information about the action's result.
	 * @return					The structure content of your json file.
	 */
	template <typename T>
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static T ReadStructFromJsonFile(const FString& JsonFilepath, bool& bOutSuccess, FString& OutInfoMessage)
	{
		T Result;

		/* Try to read generic json object from file. */
		const TSharedPtr<FJsonObject> JsonObject = ReadJson(JsonFilepath, bOutSuccess, OutInfoMessage);
		if (!bOutSuccess)
		{
			return T();
		}
		/* Try to convert generic json object into the desired structure. Output goes in Result structure. */
		if (!FJsonObjectConverter::JsonObjectToUStruct<T>(JsonObject.ToSharedRef(), &Result))
		{
			bOutSuccess = false;
			OutInfoMessage = "Read struct json failed – Was not able to convert the json object to your desired structure. Is it the right format / struct? - " + JsonFilepath;
			return T();
		}
		bOutSuccess = true;
		OutInfoMessage = "Read struct json succeeded - " + JsonFilepath;
		return Result;
	}

	/**
	 * Convert your structure to a string, create a json file and dump the string in it.
	 *
	 * @param JsonFilepath		The path of your json file: "C:/Temp/MyJson.json".
	 * @param Struct			The struct to want to write in the file.
	 * @param bOutSuccess		If the action was a success or not.
	 * @param OutInfoMessage	More information about the action's result
	 */
	template <typename T>
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteStructToJsonFile(const FString& JsonFilepath, const T& Struct, bool& bOutSuccess, FString& OutInfoMessage)
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
	 * @param bOutSuccess		If the action was a success or not
	 * @param OutInfoMessage	More information about the action's result
	 * @return					The import task
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static UAssetImportTask* CreateImportTask(FString SourcePath, const FString& DestinationPath, UFactory* ExtraFactory, UObject* ExtraOptions, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Editor Daly - Will not work in packaged build.
	 *
	 * Process the import task to import the assets
	 *
	 * @param ImportTask		The task you want to process
	 * @param bOutSuccess		If the action was a success or not
	 * @param OutInfoMessage	More information about the action's result
	 * @return					The Imported asset
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static UObject* ProcessImportTask(UAssetImportTask* ImportTask, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Create and process a basic import task.
	 * Will work for the basic asset types like textures and sounds:
	 * Can also be used for meshes, but you might not have all the control you want.
	 *
	 * @param SourcePath		The path of the source file: "C:/Temp//Texture.peg"
	 * @param DestinationPath	The path of the Imported asset: /Game/Folder/MyTexture"
	 * @param bOutSuccess		If the action was a success or not
	 * @param OutInfoMessage	More information about the action's result
	 * @return					The Imported asset
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static UObject* ImportAsset(FString SourcePath, FString DestinationPath, bool& bOutSuccess, FString& OutInfoMessage);

	/* ---------------------------- Data table ----------------------------- */

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Import a data table from a json or csv file.
	 *
	 * @param SourcePath		The path of the source file: "C:/Temp/My3son.json".
	 * @param DestinationPath	The path of the imported asset: "/Game/Folder/MyDataTable".
	 * @param StructClass		The structure to use for this data table.
	 * @param bOutSuccess		If the action was a success or not.
	 * @param OutInfoMessage	More information about the action's result.
	 * @return					The Imported data table.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static UDataTable* ImportDataTableFromJsonOrCsv(FString SourcePath, FString DestinationPath, UScriptStruct* StructClass, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Export a data table to a json or csv file.
	 * 
	 * @param FilePath			The path of the output file: "C:/Temp/y3son.json".
	 * @param DataTable			The data table to sport.
	 * @param bOutSuccess		If the action was a success or not.
	 * @param OutInfoMessage	More information about the action's result.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void ExportDataTableToJsonOrCsv(FString FilePath, UDataTable* DataTable, bool& bOutSuccess, FString& OutInfoMessage);

	/* ------------------------- Data table Json --------------------------- */
	
	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Read a series of struct based on a json file formatted like a data table.
	 *
	 * @param FilePath			The path of the source file: "C:/Temp/MyJson.json"
	 * @param bOutSuccess		If the action was a success or not
	 * @param OutInfoMessage	More information about the action's result
	 * @return					The structs
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static TMap<FString, FP12JsonTestStruct> ReadStructFromJsonFile_DataTableFormat(FString FilePath, bool& bOutSuccess, FString& OutInfoMessage);

	/**
	 * Editor Only - Will not work in packaged build.
	 *
	 * Write a series of struct to a json file. Will be formatted like a data table.
	 *
	 * @param FilePath			The path of the output file: "C:/Temp/My3son.json".
	 * @param RowsToStruct		The structs to write in the json.
	 * @param bOutSuccess		If the action was a success or not.
	 * @param OutInfoMessage	More information about the action's result.
	 */
	UFUNCTION(BlueprintCallable, Category = "C++ | Kinder")
	static void WriteStructToJsonFile_DataTableFormat(FString FilePath, TMap<FString, FP12JsonTestStruct> RowsToStruct, bool& bOutSuccess, FString& OutInfoMessage);
};
