// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "P16GameMode.generated.h"

class UP16CharacterClassInfoDataAsset;

UCLASS()
class PROJECT16_API AP16GameMode : public AGameModeBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UP16CharacterClassInfoDataAsset* GetCharacterClassInfo() const { return CharacterClassInfo.Get(); }

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Character Class Defaults")
	TObjectPtr<UP16CharacterClassInfoDataAsset> CharacterClassInfo = nullptr;
};
