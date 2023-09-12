// Copyright (C) 2023, IKinder

#include "P11/Public/UI/P11HUD.h"
#include "P11/Public/UI/P11MainWidget.h"
#include "Blueprint/UserWidget.h"

AP11HUD::AP11HUD()
{
	
}

void AP11HUD::BeginPlay()
{
	Super::BeginPlay();

	if (MainWidgetClass)
	{
		MainWidget = CreateWidget<UP11MainWidget>(GetWorld(), MainWidgetClass);
		if (!MainWidget)
		{
			return;
		}
		MainWidget->AddToViewport();
	}
}
