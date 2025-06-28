// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/P17GameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project17.h"
#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "Component/Combat/P17CombatPawnComponent.h"

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

UP17CombatPawnComponent* UP17GameplayAbility::GetCombatPawnComponentFromActorInfo() const
{
	return GetAvatarActorFromActorInfo()->FindComponentByClass<UP17CombatPawnComponent>();
}

UP17AbilitySystemComponent* UP17GameplayAbility::GetHeroAbilitySystemComponentFromActorInfo() const
{
	return Cast<UP17AbilitySystemComponent>(CurrentActorInfo->AbilitySystemComponent);
}

FActiveGameplayEffectHandle UP17GameplayAbility::NativeApplyEffectSpecHandleToTarget(AActor* InTarget, const FGameplayEffectSpecHandle& InSpecHandle) const
{
	UAbilitySystemComponent* TargetASC = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InTarget);
	WARN_RETURN_IF(!TargetASC, {})

	return GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*InSpecHandle.Data, TargetASC);
}

FActiveGameplayEffectHandle UP17GameplayAbility::BP_ApplyEffectSpecHandleToTarget(AActor* InTarget, const FGameplayEffectSpecHandle& InSpecHandle, EP17SuccessTypePin& OutExecs)
{
	const FActiveGameplayEffectHandle ActiveHandle = NativeApplyEffectSpecHandleToTarget(InTarget, InSpecHandle);
	OutExecs = ActiveHandle.WasSuccessfullyApplied() ? EP17SuccessTypePin::Successful : EP17SuccessTypePin::Failed;
	return ActiveHandle;
}
