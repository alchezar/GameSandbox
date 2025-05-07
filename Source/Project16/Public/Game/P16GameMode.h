// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "UI/ViewModel/P16MVVMLoadScreen.h"
#include "P16GameMode.generated.h"

class UP16SaveGame;
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
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

protected:
	virtual void BeginPlay() override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
public:
	UP16CharacterClassInfoDataAsset* GetCharacterClassInfo() const { return CharacterClassInfo; }
	UP16AbilityInfoDataAsset*        GetAbilityIfo() const { return AbilityInfo; }
	const FString&                   GetDefaultMapName() const { return DefaultMapName; }

	void          SaveSlotData(const UP16MVVMLoadSlot* LoadSlot, const int32 SlotIndex) const;
	static void   DeleteSlot(const FString& SlotName, const int32 SlotIndex);
	UP16SaveGame* GetSavedSlotData(const FString& SlotName, const int32 SlotIndex) const;
	UP16SaveGame* GetInGameSaveData() const;
	void          SaveInGameProgress(UP16SaveGame* SaveGame) const;
	void          TravelToMap(const UP16MVVMLoadSlot* LoadSlot);

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "C++ | Maps")
	FName DefaultPlayerStartTag = "Default";

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
