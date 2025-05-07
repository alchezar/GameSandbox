// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Util/P16Type.h"
#include "P16SaveGame.generated.h"

class UP16MVVMLoadSlot;

USTRUCT()
struct FP16SaveGameObject
{
	GENERATED_BODY()

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
	FName PlayerStartTag = "Start";
	UPROPERTY()
	bool bFirstLoad = true;
};

USTRUCT()
struct FP16SaveGamePlayer
{
	GENERATED_BODY()

	UPROPERTY()
	int32 Level = 1;
	UPROPERTY()
	int XP = 0;
	UPROPERTY()
	int32 SpellPoints = 0;
	UPROPERTY()
	int32 AttributePoints = 0;

	UPROPERTY()
	float Strength = 0.f;
	UPROPERTY()
	float Intelligence = 0.f;
	UPROPERTY()
	float Resilience = 0.f;
	UPROPERTY()
	float Vigor = 0.f;
};

UCLASS()
class PROJECT16_API UP16SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	void SaveGameObject(const UP16MVVMLoadSlot* LoadSlot, const int32 SlotIndex);
	void SavePlayerObject(const APlayerState* InPlayerState);
	void LoadPlayerObject(APlayerState* InPlayerState);

public:
	UPROPERTY()
	FP16SaveGameObject GameObject;
	UPROPERTY()
	FP16SaveGamePlayer PlayerObject;
};
