// Copyright © 2025, Ivan Kinder

#include "UI/HUD/P16HUD.h"

#include "Project16.h"
#include "Blueprint/UserWidget.h"
#include "UI/Controller/P16OverlayWidgetController.h"
#include "UI/Widget/P16Widget.h"

AP16HUD::AP16HUD()
{}

void AP16HUD::BeginPlay()
{
	Super::BeginPlay();
}

UP16OverlayWidgetController* AP16HUD::GetOverlayWidgetController(const FP16WidgetControllerParams& InParams)
{
	if (!OverlayWidgetController)
	{
		check(OverlayWidgetControllerClass)
		OverlayWidgetController = NewObject<UP16OverlayWidgetController>(this, OverlayWidgetControllerClass);
		OverlayWidgetController->InitWidgetController(InParams);
	}

	return OverlayWidgetController.Get();
}

void AP16HUD::InitOverlay(const FP16WidgetControllerParams& InParams)
{
	checkf(OverlayWidgetClass, L"Overlay Widget class uninitialized, fill out the Heads-up display!")
	OverlayWidget = CreateWidget<UP16Widget>(GetWorld(), OverlayWidgetClass);

	UP16OverlayWidgetController* WidgetController = GetOverlayWidgetController(InParams);
	OverlayWidget->SetWidgetController(WidgetController);
	WidgetController->BroadcastInitialValues();
	OverlayWidget->AddToViewport(P16::ZOrder::MainOverlay);
}
