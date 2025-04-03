// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16OverlayWidgetController.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "AbilitySystem/Data/P16LevelUpInfoDataAsset.h"
#include "Player/P16PlayerState.h"

void UP16OverlayWidgetController::BindCallbacksToDependencies()
{
	const UP16AttributeSet* AttributeSet = GetAuraAttributeSet();
	EARLY_RETURN_IF(!AttributeSet)

	if (AP16PlayerState* PlayerState = GetAuraPlayerState())
	{
		PlayerState->OnXPChanged.AddUObject(this, &ThisClass::OnXPChangedCallback);
		PlayerState->OnLevelChanged.AddWeakLambda(this, [this](const int32 NewLevel) -> void { OnLevelChanged.Broadcast(NewLevel); });
	}

	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnHealthChanged.Broadcast(Data.NewValue); });
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnMaxHealthChanged.Broadcast(Data.NewValue); });
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnManaChanged.Broadcast(Data.NewValue); });
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnMaxManaChanged.Broadcast(Data.NewValue); });

	if (UP16AbilitySystemComponent* AbilitySystem = GetAuraAbilitySystemComponent())
	{
		AbilitySystem->OnEffectApplied.AddUObject(this, &ThisClass::OnEffectAppliedCallback);

		// Handle both cases, when abilities are already given and when we need to wait for them.
		if (AbilitySystem->GetIsStartupAbilitiesGiven())
		{
			BroadcastEachAbilityInfo();
		}
		else
		{
			AbilitySystem->OnAbilitiesGiven.AddUObject(this, &ThisClass::BroadcastEachAbilityInfo);
		}
	}
}

void UP16OverlayWidgetController::BroadcastInitialValues()
{
	const UP16AttributeSet* AttributeSet = GetAuraAttributeSet();
	EARLY_RETURN_IF(!AttributeSet)

	OnHealthChanged.Broadcast(AttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AttributeSet->GetMaxMana());
}

void UP16OverlayWidgetController::OnEffectAppliedCallback(const FGameplayTagContainer& AssetTags)
{
	EARLY_RETURN_IF(!MessageWidgetDataTable)

	for (const FGameplayTag& Tag : AssetTags)
	{
		const FP16UIWidgetRow* Row = GetDataTableRowByTag<FP16UIWidgetRow>(MessageWidgetDataTable.Get(), Tag);
		CONTINUE_IF(!Row || !Tag.MatchesTag(MessageTag))
		OnMessageWidgetRow.Broadcast(*Row);
	}
}

void UP16OverlayWidgetController::OnXPChangedCallback(const int32 NewXP)
{
	const AP16PlayerState* PlayerState = GetAuraPlayerState();
	EARLY_RETURN_IF(!PlayerState)
	check(PlayerState->LevelUpInfos)

	const float Percent = PlayerState->LevelUpInfos->GetLevelPercentage(NewXP);
	OnXPPercentChanged.Broadcast(Percent);
}

template <typename T>
T* UP16OverlayWidgetController::GetDataTableRowByTag(UDataTable* InDataTable, const FGameplayTag& InTag)
{
	TArray<FP16UIWidgetRow*> Rows;
	InDataTable->GetAllRows(nullptr, Rows);
	for (FP16UIWidgetRow* Row : Rows)
	{
		CONTINUE_IF(!Row || Row->MessageTag != InTag)
		return Row;
	}

	_UNLIKELY return InDataTable->FindRow<FP16UIWidgetRow>(InTag.GetTagName(), nullptr);
}

template FP16UIWidgetRow* UP16OverlayWidgetController::GetDataTableRowByTag<FP16UIWidgetRow>(UDataTable* InDataTable, const FGameplayTag& InTag);
