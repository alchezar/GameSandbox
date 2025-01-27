// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15_MeleeAbility.h"

#include "Gameplay/Effects/P15_MeleeEffect.h"

UP15_MeleeAbility::UP15_MeleeAbility()
{
	CooldownGameplayEffectClass = UP15_MeleeEffect::StaticClass();
}

void UP15_MeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbilityCooldown(Handle, ActorInfo, ActivationInfo, true, nullptr);

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Doing Melee ability");
}

void UP15_MeleeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
