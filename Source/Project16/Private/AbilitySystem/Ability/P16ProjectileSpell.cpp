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

	AActor* Avatar = GetAvatarActorFromActorInfo();
	EARLY_RETURN_IF(!Avatar)
	const bool bServer = Avatar->HasAuthority();
	EARLY_RETURN_IF(!bServer)

	FTransform SpawnTransform = FTransform::Identity;
	if (Avatar->Implements<UP16CombatInterface>())
	{
		const FVector SocketLocation = IP16CombatInterface::Execute_GetCombatSocketLocation(Avatar, SocketTag);
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
	EARLY_RETURN_IF(!Projectile)

	Projectile->DamageEffectParams = MakeDamageEffectParamsFromClassDefaults();
	Projectile->FinishSpawning(SpawnTransform);
}
