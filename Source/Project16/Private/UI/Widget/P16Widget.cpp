// Copyright © 2025, Ivan Kinder

#include "UI/Widget/P16Widget.h"

void UP16Widget::NativeOnInitialized()
{
	Super::NativeOnInitialized();
}

void UP16Widget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UP16Widget::SetWidgetController(UObject* InWidgetController)
{
	WidgetController = InWidgetController;
	OnWidgetControllerSet();
}
