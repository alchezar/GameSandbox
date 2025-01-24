// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P12SaveSubsystemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UP12SaveSubsystemInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT12_API IP12SaveSubsystemInterface
{
	GENERATED_BODY()

public:
	/* Use this method instead of AActor::BeginPlay for logic of savable actors.
	 * It will always be called after AActor::BeginPlay(). */
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "C++ | SaveSubsystem")
	void OnLevelDeserialized();
	virtual void OnLevelDeserialized_Implementation() PURE_VIRTUAL(AP12SaveSubsystemInterface::OnLevelDeserialized_Implementation, /* Nothing to return */);
};
