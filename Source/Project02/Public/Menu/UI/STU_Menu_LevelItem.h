// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Game/STU_CoreTypes.h"
#include "STU_Menu_LevelItem.generated.h"

class UButton;
class UTextBlock;
class UImage;

UCLASS()
class PROJECT02_API USTU_Menu_LevelItem : public UUserWidget
{
	GENERATED_BODY()

public:
	FOnLevelSelectedSignature OnLevelSelected;

	FLevelData GetLevelData() const;
	void SetLevelData(const FLevelData& Data);
	void SetSelected(bool bSelected);

protected:
	virtual void NativeOnInitialized() override;

private:
	UFUNCTION()
	void OnLevelItemClicked();
	UFUNCTION()
	void OnLevelItemHovered();
	UFUNCTION()
	void OnLevelItemUnHovered();

protected:
	UPROPERTY(meta = (BindWidget))
	UButton* LevelSelectButton;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* LevelNameTextBlock;
	UPROPERTY(meta = (BindWidget))
	UImage* LevelImage;
	UPROPERTY(meta = (BindWidget))
	UImage* FrameImage;

private:
	FLevelData LevelData;
};
