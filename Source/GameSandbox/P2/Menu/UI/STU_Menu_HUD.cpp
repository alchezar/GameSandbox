// Copyright (C) 2023, IKinder

#include "STU_Menu_HUD.h"
#include "Blueprint/UserWidget.h"
#include "Menu/UI/STU_MenuWidget.h"

void ASTU_Menu_HUD::BeginPlay()
{
	Super::BeginPlay();

	if (USTU_BaseWidget* MenuWidget = CreateWidget<USTU_BaseWidget>(GetWorld(), MenuWidgetClass))
	{
		MenuWidget->AddToViewport();
		MenuWidget->PlayShow();
	}
	
}
