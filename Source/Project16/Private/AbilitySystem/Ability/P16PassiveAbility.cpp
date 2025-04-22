// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16PassiveAbility.h"

#include "AbilitySystem/P16AbilitySystemComponent.h"

void UP16PassiveAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (UP16AbilitySystemComponent* AbilitySystem = Cast<UP16AbilitySystemComponent>(GetAbilitySystemComponentFromActorInfo()))
	{
		AbilitySystem->OnDeactivatePassiveAbility.AddUObject(this, &ThisClass::ReceiveDeactivate);
	}
}

void UP16PassiveAbility::ReceiveDeactivate(const FGameplayTag& AbilityTag)
{
	if (GetAssetTags().HasTagExact(AbilityTag))
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, true);
	}
}
