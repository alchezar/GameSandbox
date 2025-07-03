// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/P17EnemyGameplayAbility.h"

#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "Character/P17CharacterEnemy.h"
#include "Component/Combat/P17CombatEnemyComponent.h"

AP17CharacterEnemy* UP17EnemyGameplayAbility::GetEnemyCharacterFromActorInfo()
{
	if (!CachedEnemyCharacter.IsValid())
	{
		CachedEnemyCharacter = Cast<AP17CharacterEnemy>(CurrentActorInfo->AvatarActor);
	}

	return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UP17CombatEnemyComponent* UP17EnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
	if (!CachedEnemyCombatComponent.IsValid())
	{
		CachedEnemyCombatComponent = Cast<UP17CombatEnemyComponent>(GetCombatPawnComponentFromActorInfo());
	}

	return CachedEnemyCombatComponent.IsValid() ? CachedEnemyCombatComponent.Get() : nullptr;
}

// ReSharper disable once CppPassValueParameterByConstReference
FGameplayEffectSpecHandle UP17EnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InEffectClass, const FScalableFloat& InScalableDamage) const
{
	WARN_RETURN_IF(!InEffectClass, {})

	const UP17AbilitySystemComponent* EnemyASC = GetProjectAbilitySystemComponentFromActorInfo();
	AActor* Avatar = GetAvatarActorFromActorInfo();
	WARN_RETURN_IF(!Avatar || !EnemyASC, {})

	FGameplayEffectContextHandle Context = EnemyASC->MakeEffectContext();
	Context.SetAbility(this);
	Context.AddSourceObject(Avatar);
	Context.AddInstigator(Avatar, Avatar);

	FGameplayEffectSpecHandle ResultSpec = EnemyASC->MakeOutgoingSpec(InEffectClass, GetAbilityLevel(), Context);
	ResultSpec.Data->SetSetByCallerMagnitude(
		P17::Tags::Shared_SetByCaller_Damage_Base,
		InScalableDamage.GetValueAtLevel(GetAbilityLevel()));

	return ResultSpec;
}
