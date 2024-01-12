// Copyright © 2024, IKinder

#include "P13/Public/UI/MainMenu/P13LevelButtonWidget.h"

#include "Components/Button.h"
#include "Components/Image.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"
#include "P13/Public/Library/P13Types.h"

void UP13LevelButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();
	LevelButton->OnReleased.AddDynamic(this, &ThisClass::OnLevelButtonPressed);
}

void UP13LevelButtonWidget::InitLevelButton(const FP13LevelSelect* NewLevel)
{
	LevelAddress = NewLevel->Address;
	LevelName->SetText(NewLevel->Name);
	LevelIcon->SetBrushFromTexture(NewLevel->Icon);
}

void UP13LevelButtonWidget::OnLevelButtonPressed()
{
	UGameplayStatics::OpenLevel(GetWorld(), LevelAddress);
}
