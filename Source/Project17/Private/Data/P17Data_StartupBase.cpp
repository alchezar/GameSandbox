// Copyright © 2025, Ivan Kinder

#include "Data/P17Data_StartupBase.h"

#include "Project17.h"
#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "AbilitySystem/Abilities/Base/P17GameplayAbility.h"

void UP17Data_StartupBase::GiveToAbilitySystemComponent(UP17AbilitySystemComponent* InASC, const int32 InLevel)
{
	WARN_RETURN_IF(!InASC,);
	GrandAbilities(ActivateOnGivenAbilities, InASC, InLevel);
	GrandAbilities(ReactiveAbilities, InASC, InLevel);
	GrandEffects(StartupEffects, InASC, InLevel);
}

void UP17Data_StartupBase::GrandAbilities(const TArray<TSubclassOf<UP17GameplayAbility>>& InAbilitiesToGive, UP17AbilitySystemComponent* InASC, const int32 InLevel)
{
	RETURN_IF(InAbilitiesToGive.IsEmpty(),)

	for (const TSubclassOf<UP17GameplayAbility>& Ability : InAbilitiesToGive)
	{
		CONTINUE_IF(!Ability)
		InASC->GiveAbility(FGameplayAbilitySpec {Ability, InLevel, INDEX_NONE, InASC->GetAvatarActor()});
	}
}

void UP17Data_StartupBase::GrandEffects(const TArray<TSubclassOf<UGameplayEffect>>& InEffectsToApply, UP17AbilitySystemComponent* InASC, const int32 InLevel)
{
	RETURN_IF(InEffectsToApply.IsEmpty(),)

	for (const TSubclassOf<UGameplayEffect>& EffectClass : StartupEffects)
	{
		CONTINUE_IF(!EffectClass)

		const UGameplayEffect* EffectCDO = EffectClass->GetDefaultObject<UGameplayEffect>();
		InASC->ApplyGameplayEffectToSelf(EffectCDO, InLevel, InASC->MakeEffectContext());
	}
}
