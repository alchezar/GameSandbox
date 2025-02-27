// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16WidgetController.h"

void UP16WidgetController::InitWidgetController(const FP16WidgetControllerParams& InParams)
{
	SetWidgetControllerParams(InParams);
	BindCallbacksToDependencies();
}

void UP16WidgetController::SetWidgetControllerParams(const FP16WidgetControllerParams& InParams)
{
	Params = InParams;
}

void UP16WidgetController::BindCallbacksToDependencies()
{
	// Empty.
}

void UP16WidgetController::BroadcastInitialValues()
{
	// Empty.
}
