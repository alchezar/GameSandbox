// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P12Library.generated.h"

UCLASS()
class PROJECT12_API UP12Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	static TArray<FString> GetAllConsoleVariables();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebug();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebugPrintScreen();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebugLegAlignment();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebugLedgeDetection();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebugText();
	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetCanDrawDebugFire();

	UFUNCTION(BlueprintCallable, Category = "PetProjects")
	static void DrawDebugLineTrace(const UWorld* World, const FHitResult& HitResult, const bool bDraw, const bool bOnTick = true);
	UFUNCTION(BlueprintCallable, Category = "PetProjects")
	static void DrawDebugCapsuleTrace(const UWorld* World, const FHitResult& HitResult, const float Radius, const float HalfHeight, const FColor Color, const bool bDraw);
	UFUNCTION(BlueprintCallable, Category = "PetProjects")
	static void DrawDebugDirectionalCapsule(const UWorld* World, FP12LedgeDescription& LedgeDescription, const float Radius, const float HalfHeight, const FVector& Location, const bool bDraw);
	UFUNCTION(BlueprintCallable, Category = "PetProjects")
	static void DrawDebugText(const UWorld* World, const FVector& Location, const FString& Text, const bool bDraw, const bool bOnTick = false);
	UFUNCTION(BlueprintCallable, Category = "PetProjects")
	static void DrawPrintString(const UWorld* World, const FString& Text, const bool bDraw, const bool bOnTick = false);
	UFUNCTION(BlueprintCallable, Category = "PetProjects")
	static void DrawDebugSphereTrace(const UWorld* World, const FHitResult& HitResult, const float Radius, const FVector& Start, const FVector& End, const bool bDraw, const bool bOnTick = false);

	static void FixedTurn(FRotator& DesiredRotation, const FRotator& CurrentRotation, const FRotator& DeltaRot);
};
