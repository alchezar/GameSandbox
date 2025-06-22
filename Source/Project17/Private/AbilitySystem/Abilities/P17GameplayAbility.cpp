// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/P17GameplayAbility.h"

#include "AbilitySystemComponent.h"
#include "Project17.h"

void UP17GameplayAbility::OnGiveAbility(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpec& Spec)
{
	Super::OnGiveAbility(ActorInfo, Spec);

	RETURN_IF(!ActorInfo || Spec.IsActive(),)

	if (ActivationPolicy == EP17AbilityActivationPolicy::OnGiven)
	{
		ActorInfo->AbilitySystemComponent->TryActivateAbility(Spec.Handle);
	}
}

void UP17GameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	RETURN_IF(!ActorInfo,)

	if (ActivationPolicy == EP17AbilityActivationPolicy::OnGiven)
	{
		ActorInfo->AbilitySystemComponent->ClearAbility(Handle);
	}
}
