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
		AbilitySystem->OnAbilityEquipped.AddUObject(this, &ThisClass::OnAbilityEquippedCallback);
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
	StopWaitingForEquip();

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

void UP16SpellMenuWidgetController::SpellGlobeDeselected()
{
	StopWaitingForEquip();

	// In order not to display the default description, we set the ability to NoneTag.
	SelectedAbility = {.Ability = FGSGameplayTagsSingleton::Get().P16Tags.Ability.NoneTag};
	UpdateButtons();
}

void UP16SpellMenuWidgetController::SpendPointAttempt()
{
	UP16AbilitySystemComponent* AbilitySystem = GetAuraAbilitySystemComponent();
	EARLY_RETURN_IF(!AbilitySystem)
	AbilitySystem->Server_SpendSpellPoint(SelectedAbility.Ability);
}

void UP16SpellMenuWidgetController::EquipButtonPressed()
{
	const FP16AbilityInfo Info = AbilityInfo->FindAbilityInfo(SelectedAbility.Ability);
	OnWaitForEquipSelection.Broadcast(Info.TypeTag);
	bWaitingForEquipSelection = true;

	// Save selected slot if that ability is equipped.
	if (SelectedAbility.Status.MatchesTag(FGSGameplayTagsSingleton::Get().P16Tags.Ability.Status.EquippedTag))
	{
		SelectedSlotTag = GetAuraAbilitySystemComponent()->GetInputTagFromAbilityTag(SelectedAbility.Ability);
	}
}

void UP16SpellMenuWidgetController::SpellRowGlobePressed(const FGameplayTag& SlotInputTag, const FGameplayTag& TypeTag)
{
	EARLY_RETURN_IF(!bWaitingForEquipSelection)
	// Compare selected ability and slot's ability type
	// Don't equip offensive spell in a passive slot and vice versa.
	const FGameplayTag& SelectedAbilityType = AbilityInfo->FindAbilityInfo(SelectedAbility.Ability).TypeTag;
	EARLY_RETURN_IF(!SelectedAbilityType.MatchesTagExact(TypeTag))

	GetAuraAbilitySystemComponent()->Server_EquipAbility(SelectedAbility.Ability, SlotInputTag);
}

void UP16SpellMenuWidgetController::OnAbilityEquippedCallback(const FGameplayTag& AbilityTag, const FGameplayTag& SlotInputTag, const FGameplayTag& PreviousSlotInputTag, const FGameplayTag& StatusTag)
{
	const auto AbilityTags = FGSGameplayTagsSingleton::Get().P16Tags.Ability;

	// Broadcast empty info if PreviousSlotTag is valid. Only if equipping an already equipped spell.
	const FP16AbilityInfo LastSlotInfo {
		.AbilityTag = AbilityTags.NoneTag,
		.InputTag = PreviousSlotInputTag,
		.StatusTag = AbilityTags.Status.UnlockedTag
	};
	AbilityInfoDelegate.Broadcast(LastSlotInfo);

	FP16AbilityInfo Info = AbilityInfo->FindAbilityInfo(AbilityTag);
	Info.StatusTag       = StatusTag;
	Info.InputTag        = SlotInputTag;
	AbilityInfoDelegate.Broadcast(Info);

	StopWaitingForEquip();
}

void UP16SpellMenuWidgetController::UpdateButtons()
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

	auto [CurrentDescription, NextDescription] = GetAuraAbilitySystemComponent()->GetDescription(SelectedAbility.Ability);
	OnSpellGlobeSelected.Broadcast(bAllowSpend, bAllowEquip, CurrentDescription, NextDescription);
}

void UP16SpellMenuWidgetController::StopWaitingForEquip()
{
	EARLY_RETURN_IF(!bWaitingForEquipSelection)

	// Broadcast the type of last selected ability before changing it.
	const FGameplayTag SelectedAbilityTypeTag = AbilityInfo->FindAbilityInfo(SelectedAbility.Ability).TypeTag;
	StopWaitForEquipSelection.Broadcast(SelectedAbilityTypeTag);
	bWaitingForEquipSelection = false;
}
