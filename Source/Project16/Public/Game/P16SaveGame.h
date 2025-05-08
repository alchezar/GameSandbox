// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "Util/P16Type.h"
#include "P16SaveGame.generated.h"

class AP16Character;
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

USTRUCT()
struct FP16SaveActor
{
	GENERATED_BODY()

	UPROPERTY()
	FString Name = "";
	UPROPERTY()
	FTransform Transform = {};
	UPROPERTY()
	TArray<uint8> Bytes = {};

	// For `AddUnique`.
	bool operator==(const FP16SaveActor& Other) const
	{
		return Name == Other.Name;
	}
};

USTRUCT()
struct FP16SaveMap
{
	GENERATED_BODY()

	UPROPERTY()
	FString AssetName = "";
	UPROPERTY()
	TArray<FP16SaveActor> SavedActors = {};
};

/// ============================================================================
/// @name UP16SaveGame
/// ============================================================================

UCLASS()
class PROJECT16_API UP16SaveGame : public USaveGame
{
	GENERATED_BODY()

public:
	_NODISCARD
	FP16SaveMap GetSaveMapBy(const FString& InMapName) const;
	_NODISCARD
	bool GetHasSaveMap(const FString& InMapName) const;

	void SaveProgress(const FName& CheckpointTag, const AP16Character* InCharacter);
	void SaveGameObject(const UP16MVVMLoadSlot* LoadSlot, const int32 SlotIndex);
	void SavePlayerObject(const APlayerState* InPlayerState);
	void SaveAbilityObject(UAbilitySystemComponent* InAbilitySystemComponent);

	void LoadGameObject(UP16MVVMLoadSlot* LoadSlot) const;
	void LoadPlayerObject(APlayerState* InPlayerState);
	void LoadAbilityObject(UAbilitySystemComponent* InAbilitySystemComponent);

public:
	UPROPERTY()
	FP16SaveGameObject GameObject;
	UPROPERTY()
	FP16SaveGamePlayer PlayerObject;
	UPROPERTY()
	TArray<FP16SavedAbility> SavedAbilities = {};
	UPROPERTY()
	TArray<FP16SaveMap> SavedMaps = {};
};
