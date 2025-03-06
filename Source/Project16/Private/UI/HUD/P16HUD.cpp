// Copyright © 2025, Ivan Kinder

#include "UI/HUD/P16HUD.h"

#include "Project16.h"
#include "Blueprint/UserWidget.h"
#include "UI/Controller/P16AttributeMenuWidgetController.h"
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
	return GetWidgetController(OverlayWidgetController, OverlayWidgetControllerClass, InParams);
}

UP16AttributeMenuWidgetController* AP16HUD::GetAttributeMenuWidgetController(const FP16WidgetControllerParams& InParams)
{
	return GetWidgetController(AttributeMenuWidgetController, AttributeMenuWidgetControllerClass, InParams);
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

template <typename T>
T* AP16HUD::GetWidgetController(TObjectPtr<T>& InWidgetController, TSubclassOf<T> InWidgetControllerClass, const FP16WidgetControllerParams& InParams)
{
	if (!InWidgetController)
	{
		check(InWidgetControllerClass)
		InWidgetController = NewObject<T>(this, InWidgetControllerClass);
		InWidgetController->InitWidgetController(InParams);
	}

	return InWidgetController;
}

template UP16OverlayWidgetController*       AP16HUD::GetWidgetController<UP16OverlayWidgetController>(TObjectPtr<UP16OverlayWidgetController>& InWidgetController, TSubclassOf<UP16OverlayWidgetController> InWidgetControllerClass, const FP16WidgetControllerParams& InParams);
template UP16AttributeMenuWidgetController* AP16HUD::GetWidgetController<UP16AttributeMenuWidgetController>(TObjectPtr<UP16AttributeMenuWidgetController>& InWidgetController, TSubclassOf<UP16AttributeMenuWidgetController> InWidgetControllerClass, const FP16WidgetControllerParams& InParams);
