// Copyright © 2024, IKinder

#include "P13/Public/UI/MainMenu/P13MainMenuWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "P13/Public/Library/P13Types.h"
#include "P13/Public/UI/MainMenu/P13LevelButtonWidget.h"

void UP13MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	PlayButton->OnReleased.AddDynamic(this, &ThisClass::OnPlayButtonPressed);
	OptionsButton->OnReleased.AddDynamic(this, &ThisClass::OnOptionsButtonPressed);
	ExitButton->OnReleased.AddDynamic(this, &ThisClass::OnExitButtonPressed);

	ClearLevelsList();
	ClearOptionsList();
}

void UP13MainMenuWidget::OnPlayButtonPressed()
{
	ClearOptionsList();

	APlayerController* PlayerController = GetOwningPlayer();
	check(PlayerController)
	check(LevelSelectTable)

	TArray<FP13LevelSelect*> Levels;
	LevelSelectTable->GetAllRows<FP13LevelSelect>(nullptr, OUT Levels);

	for (const FP13LevelSelect* Level : Levels)
	{
		UP13LevelButtonWidget* LevelButtonWidget = CreateWidget<UP13LevelButtonWidget>(PlayerController, LevelButtonWidgetClass);
		check(LevelButtonWidget)

		LevelButtonWidget->InitLevelButton(Level);
		LevelButtonWidget->AddToViewport();
		LevelsHorizontalBox->AddChild(LevelButtonWidget);
	}
}

void UP13MainMenuWidget::OnOptionsButtonPressed()
{
	ClearLevelsList();

	//
}

void UP13MainMenuWidget::OnExitButtonPressed()
{
	if (APlayerController* PlayerController = GetOwningPlayer())
	{
		PlayerController->ConsoleCommand("quit");
	}
}

void UP13MainMenuWidget::ClearLevelsList()
{
	LevelsHorizontalBox->ClearChildren();
}

void UP13MainMenuWidget::ClearOptionsList()
{
	//	
}
