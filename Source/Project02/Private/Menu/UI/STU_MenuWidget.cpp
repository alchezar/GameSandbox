// Copyright (C) 2023, IKinder

#include "Menu/UI/STU_MenuWidget.h"

#include "Components/Button.h"
#include "Components/HorizontalBox.h"
#include "Game/STU_GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Menu/UI/STU_Menu_LevelItem.h"
#include "Sound/SoundCue.h"

void USTU_MenuWidget::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (StartGameButton)
	{
		StartGameButton->OnClicked.AddDynamic(this, &ThisClass::OnGameStart);
	}
	if (QuitGameButton)
	{
		QuitGameButton->OnClicked.AddDynamic(this, &ThisClass::OnGameQuit);
	}
	InitLevelItems();
}

void USTU_MenuWidget::OnGameStart()
{
	PlayAnimation(HideAnimation);
	UGameplayStatics::PlaySound2D(GetWorld(), StartGameSound);
}

void USTU_MenuWidget::OnAnimationFinished_Implementation(const UWidgetAnimation* Animation)
{
	if (Animation != HideAnimation) return;
	
	const USTU_GameInstance* GameInstance = GetCurrentGameInstance();
	if (!GameInstance) return;

	UGameplayStatics::OpenLevel(this, GameInstance->GetStartupLevel().LevelName);
}

void USTU_MenuWidget::OnGameQuit()
{
	UKismetSystemLibrary::QuitGame(this, GetOwningPlayer(), EQuitPreference::Quit, true);
}

void USTU_MenuWidget::InitLevelItems()
{
	const USTU_GameInstance* GameInstance = GetCurrentGameInstance();
	if (!GameInstance) return;

	checkf(GameInstance->GetLevelsData().Num() != 0, TEXT("Levels data mast not be empty!"));

	if (!LevelItemsBox) return;
	LevelItemsBox->ClearChildren();

	for (auto LevelData : GameInstance->GetLevelsData())
	{
		USTU_Menu_LevelItem* LevelItemWidget = CreateWidget<USTU_Menu_LevelItem>(GetWorld(), LevelItemWidgetClass);
		if (!LevelItemWidget) continue;

		LevelItemWidget->SetLevelData(LevelData);
		LevelItemWidget->OnLevelSelected.AddUObject(this, &ThisClass::OnLevelSelected);

		LevelItemsBox->AddChild(LevelItemWidget);
		LevelItemWidgets.Add(LevelItemWidget);
	}

	if (GameInstance->GetStartupLevel().LevelName.IsNone())
	{
		OnLevelSelected(GameInstance->GetLevelsData()[0]);
	}
	else
	{
		OnLevelSelected(GameInstance->GetStartupLevel());
	}
}

void USTU_MenuWidget::OnLevelSelected(const FLevelData& Data)
{
	USTU_GameInstance* GameInstance = GetCurrentGameInstance();
	if (!GameInstance) return;

	GameInstance->SetStartupLevel(Data);

	for (const auto LevelItemWidget : LevelItemWidgets)
	{
		if (LevelItemWidget)
		{
			const bool bSelected = Data.LevelName == LevelItemWidget->GetLevelData().LevelName;
			LevelItemWidget->SetSelected(bSelected);
		}
	}
}

USTU_GameInstance* USTU_MenuWidget::GetCurrentGameInstance() const
{
	if (!GetWorld()) return nullptr;

	return GetWorld()->GetGameInstance<USTU_GameInstance>();
}
