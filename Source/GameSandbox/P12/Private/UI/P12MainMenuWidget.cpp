// Copyright (C) 2023, IKinder

#include "P12/Public/UI/P12MainMenuWidget.h"

#include "Components/Button.h"
#include "P12/Public/Game/P12HUD.h"

void UP12MainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();

	ReturnButton->OnClicked.AddDynamic(this, &ThisClass::OnReturnButtonClickedHandle);
}

void UP12MainMenuWidget::CacheHUD(const AP12HUD* HUD)
{
	CachedHUD = HUD;	
}

void UP12MainMenuWidget::OnReturnButtonClickedHandle()
{
	CachedHUD->ShowMainMenu(false);
}
