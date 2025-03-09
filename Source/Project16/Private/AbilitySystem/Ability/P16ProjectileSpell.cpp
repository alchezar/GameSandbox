// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16ProjectileSpell.h"

#include "Project16.h"
#include "Actor/P16Projectile.h"
#include "Interface/P16CombatInterface.h"

void UP16ProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!ProjectileClass)
	{
		CancelAbility(Handle, ActorInfo, ActivationInfo, true);
	}

	const bool bServer = HasAuthority(&ActivationInfo);
	EARLY_RETURN_IF(!bServer)

	FTransform SpawnTransform = FTransform::Identity;
	// TODO: Set the projectile's rotation.
	if (const TScriptInterface<IP16CombatInterface> CombatInterface = GetAvatarActorFromActorInfo())
	{
		SpawnTransform.SetLocation(CombatInterface->GetCombatSocketLocation());
	}

	AP16Projectile* Projectile = GetWorld()->SpawnActorDeferred<AP16Projectile>(
		ProjectileClass,
		SpawnTransform,
		GetOwningActorFromActorInfo(),
		Cast<APawn>(GetOwningActorFromActorInfo()),
		ESpawnActorCollisionHandlingMethod::AlwaysSpawn);

	// TODO: Give the projectile a gameplay effect spec for causing damage.
	Projectile->FinishSpawning(SpawnTransform);
}
