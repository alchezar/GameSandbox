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
}

void AP11HUD::DrawUI()
{
	if (!MainWidgetClass)
	{
		return;
	}
	if (MainWidget)
	{
		DeleteUI();
	}
	MainWidget = CreateWidget<UP11MainWidget>(PlayerOwner, MainWidgetClass);
	if (!MainWidget)
	{
		return;
	}
	MainWidget->AddToViewport();
}

void AP11HUD::DeleteUI()
{
	if (!MainWidget)
	{
		return;
	}
	MainWidget->RemoveFromParent();
	MainWidget = nullptr;
}
