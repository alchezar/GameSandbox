// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "Actor/P16Projectile.h"
#include "Interface/P16CombatInterface.h"

void UP16ProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UP16ProjectileSpell::SpawnProjectile(const FVector& InTargetLocation, const FGameplayTag& SocketTag, const bool bOrientPitch)
{
	if (!ProjectileClass)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

	const bool bServer = GetAvatarActorFromActorInfo()->HasAuthority();
	EARLY_RETURN_IF(!bServer)

	FTransform SpawnTransform = FTransform::Identity;
	if (GetAvatarActorFromActorInfo()->Implements<UP16CombatInterface>())
	{
		const FVector SocketLocation = IP16CombatInterface::Execute_GetCombatSocketLocation(GetAvatarActorFromActorInfo(), SocketTag);
		FRotator      Rotation       = (InTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch               = bOrientPitch ? Rotation.Pitch : 0.f;

		SpawnTransform.SetLocation(SocketLocation);
		SpawnTransform.SetRotation(Rotation.Quaternion());
	}

	AP16Projectile* Projectile = GetWorld()->SpawnActorDeferred<AP16Projectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	if (const UAbilitySystemComponent* AbilitySystem = UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetAvatarActorFromActorInfo()))
	{
		FGameplayEffectContextHandle ContextHandle = AbilitySystem->MakeEffectContext();
		ContextHandle.SetAbility(this);
		ContextHandle.AddSourceObject(Projectile);

		const FGameplayEffectSpecHandle SpecHandle = AbilitySystem->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);
		for (const auto& [DamageTag, ScalableFloat] : DamageTypes)
		{
			const float Magnitude = ScalableFloat.GetValueAtLevel(GetAbilityLevel());
			SpecHandle.Data->SetSetByCallerMagnitude(DamageTag, Magnitude);
		}

		Projectile->DamageEffectSpecHandle = SpecHandle;
	}

	Projectile->FinishSpawning(SpawnTransform);
}
