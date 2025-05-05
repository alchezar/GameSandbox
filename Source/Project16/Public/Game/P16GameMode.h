// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/ViewModel/P16MVVMLoadScreen.h"
#include "P16GameMode.generated.h"

class UP16LoadScreenSaveGame;
class USaveGame;
class UP16MVVMLoadSlot;
class UP16AbilityInfoDataAsset;
class UP16CharacterClassInfoDataAsset;

UCLASS()
class PROJECT16_API AP16GameMode : public AGameModeBase
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
public:
	AP16GameMode();

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UP16CharacterClassInfoDataAsset* GetCharacterClassInfo() const { return CharacterClassInfo; }
	UP16AbilityInfoDataAsset*        GetAbilityIfo() const { return AbilityInfo; }
	const FString&                   GetDefaultMapName() const { return DefaultMapName; }

	void                    SaveSlotData(const UP16MVVMLoadSlot* LoadSlot, const int32 SlotIndex) const;
	static void             DeleteSlot(const FString& SlotName, const int32 SlotIndex);
	UP16LoadScreenSaveGame* GetSavedSlotData(const FString& SlotName, const int32 SlotIndex) const;
	void                    TravelToMap(const UP16MVVMLoadSlot* LoadSlot);;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
protected:
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Character Class Defaults")
	TObjectPtr<UP16CharacterClassInfoDataAsset> CharacterClassInfo = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Ability Info")
	TObjectPtr<UP16AbilityInfoDataAsset> AbilityInfo = nullptr;
	UPROPERTY(EditDefaultsOnly, Category = "C++ | Save Game")
	TSubclassOf<USaveGame> LoadScreenSaveGameClass = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Maps")
	FString DefaultMapName = "Default";
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Maps")
	TMap<FString, TSoftObjectPtr<UWorld>> Maps = {};
};
