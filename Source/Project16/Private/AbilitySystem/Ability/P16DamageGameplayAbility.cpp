// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16DamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "Util/P16Type.h"

void UP16DamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);
	for (auto [Tag, ScalableFloat] : DamageTypes)
	{
		const float Magnitude = ScalableFloat.GetValueAtLevel(GetAbilityLevel());
		DamageSpecHandle.Data->SetSetByCallerMagnitude(Tag, Magnitude);
	}

	UAbilitySystemComponent* TargetAbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);
	EARLY_RETURN_IF(!TargetAbilitySystem)

	GetAbilitySystemComponentFromActorInfo()->ApplyGameplayEffectSpecToTarget(*DamageSpecHandle.Data, TargetAbilitySystem);
}

FP16TaggedMontage UP16DamageGameplayAbility::GetRandomTaggedMontageFrom(const TArray<FP16TaggedMontage>& Montages)
{
	EARLY_RETURN_VALUE_IF(Montages.IsEmpty(), {})

	const int32 RandomIndex = FMath::Rand() % Montages.Num();
	return Montages[RandomIndex];
}
