// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15BaseAbility.h"

#include "Player/P15Character.h"

UP15BaseAbility::UP15BaseAbility() {}

void UP15BaseAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	Char = Cast<AP15Character>(ActorInfo->OwnerActor.Get());
	check(Char);
}

void UP15BaseAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

FP15AbilityInfo UP15BaseAbility::GetAbilityInfo() const
{
	const UGameplayEffect* CooldownEffect = GetCooldownGameplayEffect();
	const UGameplayEffect* CostEffect     = GetCostGameplayEffect();

	float Cooldown = 0.f;
	float Cost     = 0.f;

	EP15AbilityCostType CostType = EP15AbilityCostType::Mana;

	if (CooldownEffect && !GetIsInstant(CooldownEffect))
	{
		CooldownEffect->DurationMagnitude.GetStaticMagnitudeIfPossible(1.f, Cooldown);
	}
	if (CostEffect && !GetIsInstant(CostEffect) && !CostEffect->Modifiers.IsEmpty())
	{
		const FGameplayModifierInfo ModifierInfo = CostEffect->Modifiers[0];
		ModifierInfo.ModifierMagnitude.GetStaticMagnitudeIfPossible(1.f, Cost);
		CostType = GetCostTypeByName(ModifierInfo.Attribute.AttributeName);
	}

	FP15AbilityInfo Info = FP15AbilityInfo::Default()
		.SetCooldown(Cooldown)
		.SetCost(Cost)
		.SetCostType(CostType)
		.SetMaterial(UIMaterial.Get())
		.SetAbilityClass(GetClass());

	return Info;
}

bool UP15BaseAbility::GetIsInstant(const UGameplayEffect* InEffect) const
{
	return InEffect->DurationPolicy != EGameplayEffectDurationType::Instant;
}

EP15AbilityCostType UP15BaseAbility::GetCostTypeByName(const FString& InName) const
{
	EP15AbilityCostType Result = EP15AbilityCostType::Health;

	if (InName == "Mana")
	{
		Result = EP15AbilityCostType::Mana;
	}
	else if (InName == "Strength")
	{
		Result = EP15AbilityCostType::Strength;
	}

	return Result;
}
