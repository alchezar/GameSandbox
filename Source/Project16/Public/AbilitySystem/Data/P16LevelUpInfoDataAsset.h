// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "Util/P16Type.h"
#include "P16LevelUpInfoDataAsset.generated.h"

UCLASS()
class PROJECT16_API UP16LevelUpInfoDataAsset : public UDataAsset
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	int32 FindLevelForXP(const int32 XP);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TArray<FP16LevelUpInfo> LevelUpInfos;
};
