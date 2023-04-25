// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "STU_SoundFuncLib.generated.h"

UCLASS()
class GAMESANDBOX_API USTU_SoundFuncLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	static void SetSoundClassVolume(USoundClass* SoundClass, float Volume);
	UFUNCTION(BlueprintCallable)
	static void ToggleSoundClassVolume(USoundClass* SoundClass);
};