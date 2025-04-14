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
	FP16DamageEffectParams DamageEffectParams {
		.WorldContext = GetAvatarActorFromActorInfo(),
		.DamageEffectClass = DamageEffectClass,
		.SourceAbilitySystemComponent = GetAbilitySystemComponentFromActorInfo(),
		.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor),
		.BaseDamage = DamageInfo.Damage.GetValueAtLevel(GetAbilityLevel()),
		.AbilityLevel = GetAbilityLevel(),
		.DamageType = DamageInfo.Tag,
		.Debuff = DebuffInfo,
		.DeathImpulse = DeathImpulse,
		.Knockback = KnockbackForce
	};

	UpdateDamageEffectParams(TargetActor, DamageEffectParams);

	return DamageEffectParams;
}

void UP16DamageGameplayAbility::UpdateDamageEffectParams(AActor* TargetActor, FP16DamageEffectParams& DamageEffectParams)
{
	EARLY_RETURN_IF(!TargetActor)

	const AActor* Avatar   = DamageEffectParams.SourceAbilitySystemComponent->GetAvatarActor();
	const FVector ToTarget = (TargetActor->GetActorLocation() - Avatar->GetActorLocation()).GetSafeNormal();

	constexpr float Angle              = -25.f;
	const FVector   KnockbackDirection = ToTarget.RotateAngleAxis(Angle, Avatar->GetActorRightVector());

	// Ability system.
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	// Death impulse.
	DamageEffectParams.DeathImpulse.Velocity = KnockbackDirection * DamageEffectParams.DeathImpulse.Magnitude;

	// Knockback force.
	if (DamageEffectParams.Knockback.Chance > FMath::RandRange(0.f, 100.f))
	{
		DamageEffectParams.Knockback.Velocity = KnockbackDirection * DamageEffectParams.Knockback.Magnitude;
	}
}
