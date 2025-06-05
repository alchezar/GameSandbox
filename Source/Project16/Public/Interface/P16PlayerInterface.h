// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "P16PlayerInterface.generated.h"

UINTERFACE()
class UP16PlayerInterface : public UInterface
{
	GENERATED_BODY()
};

class PROJECT16_API IP16PlayerInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintNativeEvent)
	int32 GetXP() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetLevelFor(const int32 XP) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePointsReward(const int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPointsReward(const int32 Level) const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetAttributePoints() const;

	UFUNCTION(BlueprintNativeEvent)
	int32 GetSpellPoints() const;

	UFUNCTION(BlueprintNativeEvent)
	void AddToXP(const int32 XP);

	UFUNCTION(BlueprintNativeEvent)
	void AddToLevel(const int32 Level);

	UFUNCTION(BlueprintNativeEvent)
	void AddAttributePoints(const int32 InAttributePoints);

	UFUNCTION(BlueprintNativeEvent)
	void AddSpellPoints(const int32 InSpellPoints);

	UFUNCTION(BlueprintNativeEvent)
	void LevelUp();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void ToggleMagicCircle(const bool bOn, UMaterialInterface* DecalMaterial = nullptr);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SaveProgress(const FName& CheckpointTag);
};
