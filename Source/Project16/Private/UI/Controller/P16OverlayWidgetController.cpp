// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16OverlayWidgetController.h"

#include "AttributeSet.h"
#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AttributeSet.h"

void UP16OverlayWidgetController::BindCallbacksToDependencies()
{
	const UP16AttributeSet* AttributeSet = Cast<UP16AttributeSet>(Params.AttributeSet.Get());
	EARLY_RETURN_IF(!AttributeSet)

	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnHealthChanged.Broadcast(Data.NewValue); });
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnMaxHealthChanged.Broadcast(Data.NewValue); });
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnManaChanged.Broadcast(Data.NewValue); });
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute())
		.AddWeakLambda(this, [this](const FOnAttributeChangeData& Data) -> void { OnMaxManaChanged.Broadcast(Data.NewValue); });

	if (UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(Params.AbilitySystemComponent))
	{
		AbilitySystem->OnEffectApplied.AddWeakLambda(this, [this](const FGameplayTagContainer& AssetTags) -> void
		{
			EARLY_RETURN_IF(!MessageWidgetDataTable)
			for (const FGameplayTag& Tag : AssetTags)
			{
				const FP16UIWidgetRow* Row = GetDataTableRowByTag<FP16UIWidgetRow>(MessageWidgetDataTable.Get(), Tag);
				CONTINUE_IF(!Row || !Tag.MatchesTag(MessageTag))
				OnMessageWidgetRow.Broadcast(*Row);
			}
		});
	}
}

void UP16OverlayWidgetController::BroadcastInitialValues()
{
	const UP16AttributeSet* AttributeSet = Cast<UP16AttributeSet>(Params.AttributeSet.Get());
	EARLY_RETURN_IF(!AttributeSet)

	OnHealthChanged.Broadcast(AttributeSet->GetHealth());
	OnMaxHealthChanged.Broadcast(AttributeSet->GetMaxHealth());
	OnManaChanged.Broadcast(AttributeSet->GetMana());
	OnMaxManaChanged.Broadcast(AttributeSet->GetMaxMana());
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

template
FP16UIWidgetRow* UP16OverlayWidgetController::GetDataTableRowByTag<FP16UIWidgetRow>(UDataTable* InDataTable, const FGameplayTag& InTag);
