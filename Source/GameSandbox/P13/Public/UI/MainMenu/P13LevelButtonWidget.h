// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13LevelButtonWidget.generated.h"

struct FP13LevelSelect;
class UButton;
class UImage;
class UTextBlock;

UCLASS()
class GAMESANDBOX_API UP13LevelButtonWidget : public UUserWidget
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
public:
	void InitLevelButton(const FP13LevelSelect* NewLevel);

protected:
	UFUNCTION()
	void OnLevelButtonPressed();
	/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
	 *                               Variables                               *
	 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelName;
	UPROPERTY(meta = (BindWidget))
	UImage* LevelIcon;
	UPROPERTY(meta = (BindWidget))
	UButton* LevelButton;

private:
	FName LevelAddress;
};
