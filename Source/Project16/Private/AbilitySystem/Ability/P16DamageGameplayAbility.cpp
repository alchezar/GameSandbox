// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16DamageGameplayAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "Util/P16Type.h"

void UP16DamageGameplayAbility::CauseDamage(AActor* TargetActor)
{
	const FGameplayEffectSpecHandle DamageSpecHandle = MakeOutgoingGameplayEffectSpec(DamageEffectClass);

	const float Magnitude = GetDamageAtLevel(GetAbilityLevel());
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
		.BaseDamage = GetDamageAtLevel(GetAbilityLevel()),
		.AbilityLevel = GetAbilityLevel(),
		.DamageType = DamageInfo.Tag,
		.Debuff = DebuffInfo,
		.DeathImpulse = DeathImpulse,
		.Knockback = KnockbackForce,
		.RadialParams = RadialDamageParams
	};

	const FVector CauserLocation = RadialDamageParams.bRadial
		? RadialDamageParams.Origin
		: GetAbilitySystemComponentFromActorInfo()->GetAvatarActor()->GetActorLocation();
	UpdateTricksVelocity(CauserLocation, TargetActor, DamageEffectParams);

	return DamageEffectParams;
}

float UP16DamageGameplayAbility::GetDamageAtLevel(const int32 InLevel) const
{
	return DamageInfo.Damage.GetValueAtLevel(InLevel);
}

void UP16DamageGameplayAbility::UpdateRadialDamageOrigin(const FVector& NewOrigin)
{
	RadialDamageParams.Origin = NewOrigin;
}

void UP16DamageGameplayAbility::UpdateTricksVelocity(const FVector& CauserLocation, AActor* TargetActor, FP16DamageEffectParams& DamageEffectParams)
{
	EARLY_RETURN_IF(!TargetActor)

	const FVector   ToTarget   = (TargetActor->GetActorLocation() - CauserLocation).GetSafeNormal();
	constexpr float PitchAngle = 25.f;
	const FVector   Direction  = ToTarget.RotateAngleAxis(PitchAngle, TargetActor->GetActorRightVector());

	// Ability system.
	DamageEffectParams.TargetAbilitySystemComponent = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(TargetActor);

	// Death impulse.
	DamageEffectParams.DeathImpulse.Velocity = Direction * DamageEffectParams.DeathImpulse.Magnitude;

	// Knockback force.
	if (DamageEffectParams.Knockback.Chance > FMath::RandRange(0.f, 100.f))
	{
		DamageEffectParams.Knockback.Velocity = Direction * DamageEffectParams.Knockback.Magnitude;
	}
}
