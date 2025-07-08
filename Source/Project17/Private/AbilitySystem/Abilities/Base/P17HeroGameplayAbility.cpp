// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Abilities/Base/P17HeroGameplayAbility.h"

#include "AbilitySystem/P17AbilitySystemComponent.h"
#include "Component/Combat/P17CombatHeroComponent.h"
#include "Public/Character/P17CharacterHero.h"
#include "Public/Controller/P17ControllerHero.h"
#include "Util/P17GameplayTags.h"

AP17CharacterHero* UP17HeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if (!CachedHeroCharacter.IsValid())
	{
		CachedHeroCharacter = Cast<AP17CharacterHero>(CurrentActorInfo->AvatarActor);
	}

	return CachedHeroCharacter.IsValid() ? CachedHeroCharacter.Get() : nullptr;
}

AP17ControllerHero* UP17HeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedHeroController.IsValid())
	{
		CachedHeroController = Cast<AP17ControllerHero>(CurrentActorInfo->PlayerController);
	}

	return CachedHeroController.IsValid() ? CachedHeroController.Get() : nullptr;
}

UP17CombatHeroComponent* UP17HeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	if (!CachedHeroCombatComponent.IsValid())
	{
		CachedHeroCombatComponent = Cast<UP17CombatHeroComponent>(GetCombatPawnComponentFromActorInfo());
	}

	return CachedHeroCombatComponent.IsValid() ? CachedHeroCombatComponent.Get() : nullptr;
}

// ReSharper disable once CppPassValueParameterByConstReference
FGameplayEffectSpecHandle UP17HeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> InEffectClass, const float InDamage, const FGameplayTag InAttackTag, const int32 InComboCount) const
{
	WARN_RETURN_IF(!InEffectClass, {})

	AActor* Avatar = GetAvatarActorFromActorInfo();
	const UP17AbilitySystemComponent* HeroASC = GetProjectAbilitySystemComponentFromActorInfo();
	WARN_RETURN_IF(!Avatar || !HeroASC, {})

	FGameplayEffectContextHandle Context = HeroASC->MakeEffectContext();
	Context.SetAbility(this);
	Context.AddSourceObject(Avatar);
	Context.AddInstigator(Avatar, Avatar);

	FGameplayEffectSpecHandle ResultSpec = HeroASC->MakeOutgoingSpec(InEffectClass, GetAbilityLevel(), Context);
	ResultSpec.Data->SetSetByCallerMagnitude(P17::Tags::Shared_SetByCaller_Damage_Base, InDamage);
	if (InAttackTag.IsValid())
	{
		ResultSpec.Data->SetSetByCallerMagnitude(InAttackTag, InComboCount);
	}

	return ResultSpec;
}

void UP17HeroGameplayAbility::FaceControllerTo(const AActor* Target, const float DeltaTime, const float InterpSpeed) const
{
	const AActor* Owner = GetAvatarActorFromActorInfo();
	const TWeakObjectPtr<APlayerController> Controller = CurrentActorInfo->PlayerController;
	RETURN_IF(!Owner || !Target || !Controller.IsValid(),)

	const FRotator NewRotation = (Target->GetActorLocation() - Owner->GetActorLocation()).Rotation();
	const FRotator CurrentRotation = Controller->GetControlRotation();
	const FRotator TargetRotation = FMath::RInterpTo(CurrentRotation, NewRotation, DeltaTime, InterpSpeed);

	Controller->SetControlRotation({CurrentRotation.Pitch, TargetRotation.Yaw, 0.f});
}
