// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16SpellMenuWidgetController.h"

void UP16SpellMenuWidgetController::BindCallbacksToDependencies()
{}

void UP16SpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastEachAbilityInfo();
}
