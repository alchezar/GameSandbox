// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "P12StreamingSubsystemVolume.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P12StreamingSubsystemUtils.generated.h"

UCLASS()
class GAMESANDBOX_API UP12StreamingSubsystemUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void CheckCharacterOverlapStreamingSubsystemVolume(ACharacter* Character);
	UFUNCTION(BlueprintCallable, Category = "C++")
	static void CheckStreamingSubsystemVolumeOverlapCharacter(AP12StreamingSubsystemVolume* StreamingSubsystemVolume);
};
