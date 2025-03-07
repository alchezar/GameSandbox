// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16AttributeMenuWidgetController.h"

#include "Project16.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "AbilitySystem/Data/P16AttributeInfoDataAsset.h"

void UP16AttributeMenuWidgetController::BindCallbacksToDependencies()
{
	EARLY_RETURN_IF(!Params.AttributeSet)

	for (const FP16AttributeInfo& AttributeInfo : AttributeInfoData->AttributeInfos)
	{
		auto Lambda = [this, AttributeInfo](const FOnAttributeChangeData& Data) -> void
		{
			AttributeInfoDelegate.Broadcast(AttributeInfoData->FindAttributeInfo(AttributeInfo.Tag, Params.AttributeSet.Get()));
		};
		Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeInfo.Attribute).AddWeakLambda(this, MoveTemp(Lambda));
	}
}

void UP16AttributeMenuWidgetController::BroadcastInitialValues()
{
	EARLY_RETURN_IF(!Params.AttributeSet.Get() || !AttributeInfoData)

	for (FP16AttributeInfo& AttributeInfo : AttributeInfoData->AttributeInfos)
	{
		const FP16AttributeInfo Info = AttributeInfoData->FindAttributeInfo(AttributeInfo.Tag, Params.AttributeSet.Get());
		AttributeInfoDelegate.Broadcast(Info);
	}
}
