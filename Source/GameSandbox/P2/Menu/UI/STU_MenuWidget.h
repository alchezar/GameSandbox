// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "STU_CoreTypes.h"
#include "UI/STU_BaseWidget.h"
#include "STU_MenuWidget.generated.h"

class UButton;
class UHorizontalBox;
class USTU_GameInstance;
class USTU_Menu_LevelItem;
class USoundCue;

UCLASS()
class GAMESANDBOX_API USTU_MenuWidget : public USTU_BaseWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeOnInitialized() override;
	virtual void OnAnimationFinished_Implementation(const UWidgetAnimation* Animation) override;

private:
	UFUNCTION()
	void OnGameStart();
	UFUNCTION()
	void OnGameQuit();
	void InitLevelItems();
	void OnLevelSelected(const FLevelData& Data);
	UFUNCTION()
	USTU_GameInstance* GetCurrentGameInstance() const;

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* StartGameButton;
	UPROPERTY(meta = (BindWidget))
	UButton* QuitGameButton;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelItemsBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | UI")
	TSubclassOf<USTU_Menu_LevelItem> LevelItemWidgetClass;
	UPROPERTY(meta = (BindWidgetAnim), Transient)
	UWidgetAnimation* HideAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | Sound")
	USoundCue* StartGameSound;

private:
	UPROPERTY()
	TArray<USTU_Menu_LevelItem*> LevelItemWidgets;
};
