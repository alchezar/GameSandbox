// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P12SaveSubsystemUtils.generated.h"

UCLASS()
class GAMESANDBOX_API UP12SaveSubsystemUtils : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "C++ | SaveSubsystem")
	static void BroadcastOnLevelDeserialized(ULevel* Level);
};
