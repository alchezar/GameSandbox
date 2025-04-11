// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16DamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "Util/P16Type.h"

void UP16DamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);

	const float Magnitude = DamageInfo.Damage.GetValueAtLevel(GetAbilityLevel());
	DamageSpecHandle.Data->SetSetByCallerMagnitude(DamageInfo.Tag, Magnitude);

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

FP16DamageEffectParams UP16DamageGameplayAbility::MakeDamageEffectParamsFromClassDefaults(AActor* TargetActor) const
{
	return FP16DamageEffectParams {
		.WorldContext = GetAvatarActorFromActorInfo(),
		.DamageEffectClass = DamageEffectClass,
		.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo(),
		.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor),
		.BaseDamage = DamageInfo.Damage.GetValueAtLevel(GetAbilityLevel()),
		.AbilityLevel = GetAbilityLevel(),
		.DamageType = DamageInfo.Tag,
		.Debuff = DebuffInfo
	};
}
