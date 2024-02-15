// Copyright © 2024, IKinder

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "P13MenuWidget.generated.h"

struct FP13LevelSelect;
class USlider;
class UVerticalBox;
class UImage;
class UTextBlock;
class UHorizontalBox;
class UButton;
class UP13LevelSelectWidget;
class UP13OptionListWidget;
class UP13OptionSliderWidget;

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              Main Menu                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13MenuWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
protected:
	UFUNCTION()
	virtual void OnPlayButtonPressed();
	UFUNCTION()
	void OnOptionsButtonPressed();
	UFUNCTION()
	void OnExitButtonPressed();

	void ClearLevelsList();
	void ClearOptionsList();

	/* ----------------------------- Variables ----------------------------- */
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
	TSubclassOf<UP13LevelSelectWidget> LevelButtonWidgetClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<UP13OptionListWidget> OptionsWidgetClass;

private:
	UPROPERTY()
	UP13OptionListWidget* OptionsWidget;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Level Select                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13LevelSelectWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	FORCEINLINE FName GetLevelAddress() const { return LevelAddress; }
	void InitLevelButton(const FP13LevelSelect* NewLevel);

protected:
	UFUNCTION()
	virtual void OnLevelButtonPressed();

	/* ----------------------------- Variables ----------------------------- */
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

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Option List                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13OptionListWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	void SetupLines();

protected:
	void OnEffectValueChangedHandle(FText SliderName, float NewValue);
	UFUNCTION()
	void OnCloseButtonPressedHandle();

private:
	void CreateOneLine(const FText& OptionName, const FVector2D OptionDiapason, const float CurrentValue);

	/* ----------------------------- Variables ----------------------------- */
protected:
	UPROPERTY(meta = (BindWidget))
	UVerticalBox* OptionsVerticalBox;
	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "C++")
	TSubclassOf<UP13OptionSliderWidget> OptionSliderClass;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Option Slider                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

UCLASS()
class GAMESANDBOX_API UP13OptionSliderWidget : public UUserWidget
{
	GENERATED_BODY()

	/* ------------------------------ Unreal ------------------------------- */
protected:
	virtual void NativeConstruct() override;

	/* ------------------------------- This -------------------------------- */
public:
	void InitOptionLine(const FText& OptionName, const FVector2D OptionDiapason, const float CurrentValue) const;

protected:
	UFUNCTION()
	void OnSliderValueChangedHandle(const float Value);

private:
	FText TextFromFloatWithPrecision(const float FloatValue, const int32 Precision = 2) const;

	/* ----------------------------- Variables ----------------------------- */
public:
	DECLARE_MULTICAST_DELEGATE_TwoParams(FP13OnOptionSliderValueChangedSignature, FText /*SliderName*/, float /*NewValue*/)

	FP13OnOptionSliderValueChangedSignature OnOptionSliderValueChanged;

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* NameText;
	UPROPERTY(meta = (BindWidget))
	USlider* Slider;
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ValueText;
};
