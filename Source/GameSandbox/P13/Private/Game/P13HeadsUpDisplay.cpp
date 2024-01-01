// Copyright © 2024, IKinder

#include "P13/Public/Game/P13HeadsUpDisplay.h"

#include "Blueprint/UserWidget.h"
#include "P13/Public/UI/P13InGameWidget.h"

AP13HeadsUpDisplay::AP13HeadsUpDisplay()
{
	
}

void AP13HeadsUpDisplay::BeginPlay()
{
	Super::BeginPlay();
}

void AP13HeadsUpDisplay::ShotInGame()
{
	if (!InGameWidgetClass)
	{
		return;
	}
	InGameWidgetCached = CreateWidget<UP13InGameWidget>(GetOwningPlayerController(), InGameWidgetClass);
	check(InGameWidgetCached.IsValid())
	InGameWidgetCached->AddToViewport();
}
