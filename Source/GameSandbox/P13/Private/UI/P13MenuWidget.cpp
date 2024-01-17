// Copyright © 2024, IKinder

#include "P13/Public/UI/P13MenuWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Components/Image.h"
#include "Components/Slider.h"
#include "Components/TextBlock.h"
#include "Components/VerticalBox.h"
#include "Kismet/GameplayStatics.h"
#include "P13/Public/Library/P13Types.h"

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                              Main Menu                                    *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13MenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnReleased.AddDynamic(this, &ThisClass::OnPlayButtonPressed);
	OptionsButton->OnReleased.AddDynamic(this, &ThisClass::OnOptionsButtonPressed);
	ExitButton->OnReleased.AddDynamic(this, &ThisClass::OnExitButtonPressed);

	ClearLevelsList();
	ClearOptionsList();
}

void UP13MenuWidget::OnPlayButtonPressed()
{
	ClearOptionsList();

	APlayerController* PlayerController = GetOwningPlayer();
	check(PlayerController)
	check(LevelSelectTable)

	TArray<FP13LevelSelect*> Levels;
	LevelSelectTable->GetAllRows<FP13LevelSelect>(nullptr, OUT Levels);

	for (const FP13LevelSelect* Level : Levels)
	{
		UP13LevelSelectWidget* LevelButtonWidget = CreateWidget<UP13LevelSelectWidget>(PlayerController, LevelButtonWidgetClass);
		check(LevelButtonWidget)

		LevelButtonWidget->InitLevelButton(Level);
		LevelButtonWidget->AddToViewport();
		LevelsHorizontalBox->AddChild(LevelButtonWidget);
	}
}

void UP13MenuWidget::OnOptionsButtonPressed()
{
	ClearLevelsList();
	ClearOptionsList();
	
	APlayerController* PlayerController = GetOwningPlayer();
	check(PlayerController)

	OptionsWidget = CreateWidget<UP13OptionListWidget>(PlayerController, OptionsWidgetClass);
	check(OptionsWidget);

	OptionsWidget->AddToViewport(1);
}

void UP13MenuWidget::OnExitButtonPressed()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->ConsoleCommand("quit");
	}
}

void UP13MenuWidget::ClearLevelsList()
{
	LevelsHorizontalBox->ClearChildren();
}

void UP13MenuWidget::ClearOptionsList()
{
	if (!OptionsWidget)
	{
		return;
	}
	OptionsWidget->RemoveFromParent();
	OptionsWidget = nullptr;
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Level Select                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13LevelSelectWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LevelButton->OnReleased.AddDynamic(this, &ThisClass::OnLevelButtonPressed);
}

void UP13LevelSelectWidget::InitLevelButton(const FP13LevelSelect* NewLevel)
{
	LevelAddress = NewLevel->RealName;
	LevelName->SetText(NewLevel->ShowName);
	LevelIcon->SetBrushFromTexture(NewLevel->Icon);
}

void UP13LevelSelectWidget::OnLevelButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelAddress);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                Option List                                *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13OptionListWidget::NativeConstruct()
{
	Super::NativeConstruct();
	SetupLines();
	
	CloseButton->OnReleased.AddDynamic(this, &ThisClass::OnCloseButtonPressedHandle);
}

void UP13OptionListWidget::SetupLines()
{
	OptionsVerticalBox->ClearChildren();
	
	CreateOneLine(FText::FromString("Effect"), {0.01f, 1.5f}, 1.f);
	CreateOneLine(FText::FromString("Music"), {0.01f, 2.f}, 1.f);
}

void UP13OptionListWidget::OnEffectValueChangedHandle(FText SliderName, float NewValue)
{
	if (SliderName.ToString() == "Effect")
	{
		// 

		return;
	}
	if (SliderName.ToString() == "Music")
	{
		//

		return;
	}
}

void UP13OptionListWidget::OnCloseButtonPressedHandle()
{
	RemoveFromParent();
}

void UP13OptionListWidget::CreateOneLine(const FText& OptionName, const FVector2D OptionDiapason, const float CurrentValue)
{
	UP13OptionSliderWidget* OptionLineWidget = CreateWidget<UP13OptionSliderWidget>(GetOwningPlayer(), OptionSliderClass);
	check(OptionLineWidget)

	OptionLineWidget->InitOptionLine(OptionName, OptionDiapason, CurrentValue);
	OptionLineWidget->OnOptionSliderValueChanged.AddUObject(this, &ThisClass::OnEffectValueChangedHandle);
	OptionLineWidget->AddToViewport();

	OptionsVerticalBox->AddChild(OptionLineWidget);
}

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 *                               Option Slider                               *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */

void UP13OptionSliderWidget::NativeConstruct()
{
	Super::NativeConstruct();
	Slider->OnValueChanged.AddDynamic(this, &ThisClass::OnSliderValueChangedHandle);
}

void UP13OptionSliderWidget::InitOptionLine(const FText& OptionName, const FVector2D OptionDiapason, const float CurrentValue) const
{
	NameText->SetText(OptionName);
	Slider->SetMinValue(OptionDiapason.X);
	Slider->SetMaxValue(OptionDiapason.Y);
	Slider->SetValue(CurrentValue);
	ValueText->SetText(TextFromFloatWithPrecision(CurrentValue));
}

void UP13OptionSliderWidget::OnSliderValueChangedHandle(const float Value)
{
	ValueText->SetText(TextFromFloatWithPrecision(Value));
	OnOptionSliderValueChanged.Broadcast(NameText->GetText(), Value);
}

FText UP13OptionSliderWidget::TextFromFloatWithPrecision(const float FloatValue, const int32 Precision) const
{
	return FText::FromString(FString::Printf(TEXT("%.*f"), Precision, FloatValue));
}
