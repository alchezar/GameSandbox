// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13MainMenuWidget.generated.h"

class UP13LevelButtonWidget;
class UHorizontalBox;
class UButton;

UCLASS()
class GAMESANDBOX_API UP13MainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                Super                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	virtual void NativeConstruct() override;
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                                 This                                  *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UFUNCTION()
	void OnPlayButtonPressed();
	UFUNCTION()
	void OnOptionsButtonPressed();
	UFUNCTION()
	void OnExitButtonPressed();

private:
	void ClearLevelsList();
	void ClearOptionsList();
	
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(meta = (BindWidget))
	UButton* PlayButton;
	UPROPERTY(meta = (BindWidget))
	UButton* OptionsButton;
	UPROPERTY(meta = (BindWidget))
	UButton* ExitButton;
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* LevelsHorizontalBox;

	UPROPERTY(EditAnywhere, Category = "C++")
	UDataTable* LevelSelectTable;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13LevelButtonWidget> LevelButtonWidgetClass;
	
};
