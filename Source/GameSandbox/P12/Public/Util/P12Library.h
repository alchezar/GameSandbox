// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "P12Library.generated.h"

UCLASS()
class GAMESANDBOX_API UP12Library : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "PetProjects")
	static bool GetDrawDebugAllowed();
};
