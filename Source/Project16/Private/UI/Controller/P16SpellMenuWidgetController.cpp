// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16SpellMenuWidgetController.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/Data/P16AbilityInfoDataAsset.h"
#include "Player/P16PlayerState.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

void UP16SpellMenuWidgetController::BindCallbacksToDependencies()
{
	if (UP16AbilitySystemComponent* AbilitySystem = GetAuraAbilitySystemComponent())
	{
		AbilitySystem->OnAbilityStatusChanged.AddWeakLambda(this,
			[this](const FGameplayTag& AbilityTag, const FGameplayTag& StatusTag, const int32 AbilityLevel) -> void
			{
				if (SelectedAbility.Ability.MatchesTagExact(AbilityTag))
				{
					SelectedAbility.Status = StatusTag;
					UpdateButtons();
				}

				EARLY_RETURN_IF(!AbilityInfo)
				FP16AbilityInfo Info = AbilityInfo->FindAbilityInfo(AbilityTag);
				Info.StatusTag       = StatusTag;
				AbilityInfoDelegate.Broadcast(Info);
			});
	}

	if (AP16PlayerState* PlayerState = GetAuraPlayerState())
	{
		PlayerState->OnSpellPointsChanged.AddWeakLambda(this,
			[this](const int32 SpellPoints) -> void
			{
				SpellPointsDelegate.Broadcast(SpellPoints);

				SelectedAbility.SpellPoints = SpellPoints;
				UpdateButtons();
			});
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

void UP16SpellMenuWidgetController::SpellGlobeSelected(const FGameplayTag& AbilityTag)
{
	const auto AbilityTags = FGSGameplayTagsSingleton::Get().P16Tags.Ability;
	SelectedAbility        = {AbilityTag, AbilityTags.Status.LockedTag, GetAuraPlayerState()->GetSpellPoints()};

	const FGameplayAbilitySpec* Spec = GetAuraAbilitySystemComponent()->GetSpecFromAbilityTag(AbilityTag);

	const bool bTagValid   = AbilityTag.IsValid();
	const bool bTagCorrect = !AbilityTag.MatchesTagExact(AbilityTags.NoneTag);
	const bool bSpecValid  = Spec != nullptr;
	if (bTagValid && bTagCorrect && bSpecValid)
	{
		SelectedAbility.Status = UP16AbilitySystemComponent::GetStatusFromSpec(*Spec);
	}

	UpdateButtons();
}

void UP16SpellMenuWidgetController::SpendPointAttempt()
{
	UP16AbilitySystemComponent* AbilitySystem = GetAuraAbilitySystemComponent();
	EARLY_RETURN_IF(!AbilitySystem)
	AbilitySystem->Server_SpendSpellPoint(SelectedAbility.Ability);
}

void UP16SpellMenuWidgetController::UpdateButtons() const
{
	const auto AbilityTags = FGSGameplayTagsSingleton::Get().P16Tags.Ability;

	bool bAllowSpend = SelectedAbility.SpellPoints > 0;
	bool bAllowEquip = true;

	if (SelectedAbility.Status.MatchesTagExact(AbilityTags.Status.LockedTag))
	{
		bAllowSpend = false;
		bAllowEquip = false;
	}
	else if (SelectedAbility.Status.MatchesTagExact(AbilityTags.Status.EligibleTag))
	{
		bAllowEquip = false;
	}
	OnSpellGlobeSelected.Broadcast(bAllowSpend, bAllowEquip);
}
