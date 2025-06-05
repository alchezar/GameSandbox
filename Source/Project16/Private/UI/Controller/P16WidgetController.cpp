// Copyright © 2025, Ivan Kinder

#include "UI/Controller/P16WidgetController.h"

#include "Project16.h"
#include "AbilitySystem/P16AbilitySystemComponent.h"
#include "AbilitySystem/P16AttributeSet.h"
#include "AbilitySystem/Data/P16AbilityInfoDataAsset.h"
#include "Player/P16PlayerController.h"
#include "Player/P16PlayerState.h"
#include "Util/P16Util.h"

#define DEFINE_SPECIFIC_PARAMS_GETTERS(Field, Type)      \
Type* UP16WidgetController::GetAura##Field()             \
{                                                        \
	if (!SpecificParams.Field)                           \
	{                                                    \
		SpecificParams.Field = Cast<Type>(Params.Field); \
	}                                                    \
	return SpecificParams.Field;                         \
}

DEFINE_SPECIFIC_PARAMS_GETTERS(PlayerController, AP16PlayerController);
DEFINE_SPECIFIC_PARAMS_GETTERS(PlayerState, AP16PlayerState)
DEFINE_SPECIFIC_PARAMS_GETTERS(AbilitySystemComponent, UP16AbilitySystemComponent);
DEFINE_SPECIFIC_PARAMS_GETTERS(AttributeSet, UP16AttributeSet);

void UP16WidgetController::SetWidgetControllerParams(const FP16WidgetControllerParams& InParams)
{
	Params = InParams;
}

void UP16WidgetController::InitWidgetController(const FP16WidgetControllerParams& InParams)
{
	SetWidgetControllerParams(InParams);
	BindCallbacksToDependencies();
}

void UP16WidgetController::BindCallbacksToDependencies()
{
	// Empty.
}

void UP16WidgetController::BroadcastInitialValues()
{
	// Empty.
}

void UP16WidgetController::BroadcastEachAbilityInfo()
{
	UP16AbilitySystemComponent* AbilitySystem = GetAuraAbilitySystemComponent();
	EARLY_RETURN_IF(!AbilitySystem || !AbilitySystem->GetIsStartupAbilitiesGiven())

	// Iterate over all activatable abilities within the ability system and broadcast info about them here.
	FP16ForEachAbilitySignature BroadcastDelegate;
	BroadcastDelegate.BindLambda([this](const FGameplayAbilitySpec& AbilitySpec) -> void
	{
		FP16AbilityInfo Info = AbilityInfo->FindAbilityInfo(UP16AbilitySystemComponent::GetAbilityTagFromSpec(AbilitySpec));
		Info.InputTag        = UP16AbilitySystemComponent::GetInputTagFromSpec(AbilitySpec);
		Info.StatusTag       = UP16AbilitySystemComponent::GetStatusFromSpec(AbilitySpec);

		// Broadcast each ability info to the blueprint widgets.
		AbilityInfoDelegate.Broadcast(Info);
	});

	AbilitySystem->ForEachAbility(BroadcastDelegate);
}
