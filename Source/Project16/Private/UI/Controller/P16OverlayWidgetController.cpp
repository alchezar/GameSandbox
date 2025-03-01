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
		.AddUObject(this, &ThisClass::OnHealthChangedCallback);
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxHealthAttribute())
		.AddUObject(this, &ThisClass::OnMaxHealthChangedCallback);
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetManaAttribute())
		.AddUObject(this, &ThisClass::OnManaChangedCallback);
	Params.AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(AttributeSet->GetMaxManaAttribute())
		.AddUObject(this, &ThisClass::OnMaxManaChangedCallback);

	if (UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(Params.AbilitySystemComponent))
	{
		AbilitySystem->OnEffectApplied.AddUObject(this, &ThisClass::OnEffectAppliedCallback);
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

void UP16OverlayWidgetController::OnHealthChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnHealthChanged.Broadcast(Data.NewValue);
}

void UP16OverlayWidgetController::OnMaxHealthChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnMaxHealthChanged.Broadcast(Data.NewValue);
}

void UP16OverlayWidgetController::OnManaChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnManaChanged.Broadcast(Data.NewValue);
}

void UP16OverlayWidgetController::OnMaxManaChangedCallback(const FOnAttributeChangeData& Data) const
{
	OnMaxManaChanged.Broadcast(Data.NewValue);
}

void UP16OverlayWidgetController::OnEffectAppliedCallback(const FGameplayTagContainer& AssetTags)
{
	for (const FGameplayTag& Tag : AssetTags)
	{
		// TODO: Do something with broadcast tags.
	}
}
