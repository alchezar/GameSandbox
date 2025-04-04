// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16SpellMenuWidgetController.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/Data/P16AbilityInfoDataAsset.h"
#include "Player/P16PlayerState.h"

void UP16SpellMenuWidgetController::BindCallbacksToDependencies()
{
	if (UP16AbilitySystemComponent* AbilitySystem = GetAuraAbilitySystemComponent())
	{
		AbilitySystem->OnAbilityStatusChanged.AddWeakLambda(this, [this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag) -> void
		{
			EARLY_RETURN_IF(!AbilityInfo)
			FP16AbilityInfo Info = AbilityInfo->FindAbilityInfo(AbilityTag);
			Info.StatusTag       = StatusTag;
			AbilityInfoDelegate.Broadcast(Info);
		});
	}

	if (AP16PlayerState* PlayerState = GetAuraPlayerState())
	{
		PlayerState->OnSpellPointsChanged.AddWeakLambda(this,
			[this](const int32 SpellPoints) -> void { SpellPointsDelegate.Broadcast(SpellPoints); });
	}
}

void UP16SpellMenuWidgetController::BroadcastInitialValues()
{
	BroadcastEachAbilityInfo();

	if (const AP16PlayerState* PlayerState = GetAuraPlayerState())
	{
		SpellPointsDelegate.Broadcast(PlayerState->GetSpellPoints());
	}
}
