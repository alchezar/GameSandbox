// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12CoreTypes.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P12Library.generated.h"

UCLASS()
class GAMESANDBOX_API UP12Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	static TArray<FString>  GetAllConsoleVariables();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebug();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebugPrintScreen();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebugLegAlignment();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebugLedgeDetection();
	
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static void DrawDebugLineTrace(const UWorld* World, const FHitResult& HitResult, const bool bDraw);
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static void DrawDebugCapsuleTrace(const UWorld* World, const FHitResult& HitResult, const float Radius, const float HalfHeight, const FColor Color, const bool bDraw);
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static void DrawDebugDirectionalCapsule(const UWorld* World, FP12LedgeDescription& LedgeDescription, const  float Radius, const float HalfHeight, const FVector& Location, const bool bDraw);

};
