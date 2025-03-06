// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16AttributeMenuWidgetController.h"

#include "Project16.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "AbilitySystem/Data/P16AttributeInfoDataAsset.h"

void UP16AttributeMenuWidgetController::BindCallbacksToDependencies()
{}

void UP16AttributeMenuWidgetController::BroadcastInitialValues()
{
	const UP16AttributeSet* AttributeSet = Cast<UP16AttributeSet>(Params.AttributeSet.Get());
	EARLY_RETURN_IF(!AttributeSet || !AttributeInfo)

	for (auto& [Tag, Delegate] : AttributeSet->TagsToAttributesMap)
	{
		FP16AttributeInfo Info = AttributeInfo->FindAttributeInfo(Tag);
		Info.Value             = Delegate.Execute().GetNumericValue(AttributeSet);
		AttributeInfoDelegate.Broadcast(Info);
	}
}
