// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Ability/P16ProjectileSpell.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "Actor/P16Projectile.h"
#include "Interface/P16CombatInterface.h"
#include "Root/Public/Singleton/GSGameplayTagsSingleton.h"

void UP16ProjectileSpell::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UP16ProjectileSpell::SpawnProjectile(const FVector& InTargetLocation)
{
	if (!ProjectileClass)
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true);
	}

	const bool bServer = GetAvatarActorFromActorInfo()->HasAuthority();
	EARLY_RETURN_IF(!bServer)

	FTransform SpawnTransform = FTransform::Identity;
	if (const TScriptInterface<IP16CombatInterface> CombatInterface = GetAvatarActorFromActorInfo())
	{
		const FVector SocketLocation = CombatInterface->GetCombatSocketLocation();
		FRotator      Rotation       = (InTargetLocation - SocketLocation).Rotation();
		Rotation.Pitch               = 0.f;

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

		const FGameplayEffectSpecHandle SpecHandle   = AbilitySystem->MakeOutgoingSpec(DamageEffectClass, GetAbilityLevel(), ContextHandle);
		const float                     ScaledDamage = Damage.GetValueAtLevel(GetAbilityLevel());
		SpecHandle.Data->SetSetByCallerMagnitude(FGSGameplayTagsSingleton::Get().P16Tags.Damage, ScaledDamage);

		Projectile->DamageEffectSpecHandle = SpecHandle;
	}

	Projectile->FinishSpawning(SpawnTransform);
}
