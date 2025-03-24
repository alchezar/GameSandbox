// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16GameplayAbility.h"

#include "Project16.h"
#include "Interface/P16CombatInterface.h"
#include "Interface/P16InterfaceEnemy.h"

void UP16GameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

// ReSharper disable once CppUE4BlueprintCallableFunctionMayBeConst
AActor* UP16GameplayAbility::OrientToTarget()
{
	AActor* Target = IP16InterfaceEnemy::Execute_GetCombatTarget(GetAvatarActorFromActorInfo());
	EARLY_RETURN_VALUE_IF(!Target, nullptr)

	IP16CombatInterface::Execute_UpdateFacingTo(GetAvatarActorFromActorInfo(), Target->GetActorLocation());
	return Target;
}
