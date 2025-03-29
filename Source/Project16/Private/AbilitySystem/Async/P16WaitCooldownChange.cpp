// Copyright © 2025, Ivan Kinder

#include "Project16/Public/AbilitySystem/AsyncTask/P16WaitCooldownChange.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "Algo/MaxElement.h"

UP16WaitCooldownChange* UP16WaitCooldownChange::WaitCooldownChange(UAbilitySystemComponent* InAbilitySystemComponent, const FGameplayTag& InCooldownTag)
{
	UP16WaitCooldownChange* WaitCooldownChange = NewObject<UP16WaitCooldownChange>();
	WaitCooldownChange->AbilitySystemComponent = InAbilitySystemComponent;
	WaitCooldownChange->CooldownTag            = InCooldownTag;

	if (!InAbilitySystemComponent || !InCooldownTag.IsValid())
	{
		WaitCooldownChange->EndTask();
		return nullptr;
	}

	// Callback to know when the cooldown effect has been removed.
	InAbilitySystemComponent->RegisterGameplayTagEvent(InCooldownTag)
		.AddUObject(WaitCooldownChange, &ThisClass::OnCooldownTagChangedCallback);

	// Callback to know when the cooldown effect has been applied.
	InAbilitySystemComponent->OnActiveGameplayEffectAddedDelegateToSelf
		.AddUObject(WaitCooldownChange, &ThisClass::OnActiveEffectAddedCallback);

	return WaitCooldownChange;
}

void UP16WaitCooldownChange::EndTask()
{
	EARLY_RETURN_IF(!AbilitySystemComponent || !CooldownTag.IsValid())
	AbilitySystemComponent->RegisterGameplayTagEvent(CooldownTag).RemoveAll(this);

	SetReadyToDestroy();
	MarkAsGarbage();
}

void UP16WaitCooldownChange::OnCooldownTagChangedCallback(FGameplayTag InCooldownTag, const int NewCount)
{
	if (NewCount == 0)
	{
		CooldownEnd.Broadcast(0.f);
	}
}

void UP16WaitCooldownChange::OnActiveEffectAddedCallback(UAbilitySystemComponent* TargetAbilitySystemComponent, const FGameplayEffectSpec& GameplayEffectSpec, FActiveGameplayEffectHandle ActiveGameplayEffectHandle)
{
	FGameplayTagContainer AssetTags;
	GameplayEffectSpec.GetAllAssetTags(AssetTags);
	FGameplayTagContainer GrantedTags;
	GameplayEffectSpec.GetAllGrantedTags(GrantedTags);

	EARLY_RETURN_IF(!AssetTags.HasTagExact(CooldownTag) && !GrantedTags.HasTagExact(CooldownTag));

	const FGameplayEffectQuery Query          = FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(CooldownTag.GetSingleTagContainer());
	TArray<float>              TimesRemaining = AbilitySystemComponent->GetActiveEffectsDuration(Query);

	EARLY_RETURN_IF(TimesRemaining.IsEmpty())

	const float* MaxTime = Algo::MaxElement(TimesRemaining);
	CooldownStart.Broadcast(MaxTime ? *MaxTime : TimesRemaining[0]);
}
