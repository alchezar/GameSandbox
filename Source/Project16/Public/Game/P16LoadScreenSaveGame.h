// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Util/P16Type.h"
#include "P16LoadScreenSaveGame.generated.h"

UCLASS()
class PROJECT16_API UP16LoadScreenSaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FString SlotName = "Slot_";
	UPROPERTY()
	int32 SlotIndex = 0;
	UPROPERTY()
	FString PlayerName = "Default";
	UPROPERTY()
	FString MapName = "Default";
	UPROPERTY()
	EP16SaveSlotStatus SlotStatus = EP16SaveSlotStatus::Vacant;
	UPROPERTY()
	int32 Level = 1;
};
