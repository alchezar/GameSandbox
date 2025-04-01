// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16AttributeMenuWidgetController.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "AbilitySystem/Data/P16AttributeInfoDataAsset.h"
#include "Player/P16PlayerState.h"

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

	if (AP16PlayerState* PlayerState = Cast<AP16PlayerState>(Params.PlayerState))
	{
		PlayerState->OnAttributePointsChanged.AddWeakLambda(this,
			[this](const int32 AttributePoints) -> void { AttributePointsDelegate.Broadcast(AttributePoints); });
		PlayerState->OnSpellPointsChanged.AddWeakLambda(this,
			[this](const int32 SpellPoints) -> void { SpellPointsDelegate.Broadcast(SpellPoints); });
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

	if (const AP16PlayerState* PlayerState = Cast<AP16PlayerState>(Params.PlayerState))
	{
		AttributePointsDelegate.Broadcast(PlayerState->GetAttributePoints());
		SpellPointsDelegate.Broadcast(PlayerState->GetSpellPoints());
	}
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
void UP16AttributeMenuWidgetController::UpdateAttribute(const FGameplayTag& AttributeTag)
{
	UP16AbilitySystemComponent* AbilitySystemComponent = Cast<UP16AbilitySystemComponent>(Params.AbilitySystemComponent);
	EARLY_RETURN_IF(!AbilitySystemComponent);

	AbilitySystemComponent->UpdateAttribute(AttributeTag);
}
