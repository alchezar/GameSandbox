// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15MeleeAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project15.h"
#include "Camera/CameraComponent.h"
#include "Gameplay/Effects/P15MeleeEffect.h"
#include "Player/P15Character.h"

UP15MeleeAbility::UP15MeleeAbility()
{
	CooldownGameplayEffectClass = UP15MeleeEffect::StaticClass();
}

void UP15MeleeAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	// Find the enemy ahead.
	FHitResult      HitResult;
	constexpr float TraceDistance = 1000.f;
	const FVector   Start         = Char->GetPlayerEye()->GetComponentLocation();
	const FVector   End           = Start + Char->GetPlayerEye()->GetForwardVector() * TraceDistance;
	GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Pawn);
	if (bDebug)
	{
		DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 5.f);
	}
	EARLY_RETURN_IF(!HitResult.bBlockingHit)

	Enemy = Cast<AP15Character>(HitResult.GetActor());
	EARLY_RETURN_IF(!Enemy || Enemy->GetIsDead())

	// Start the attack animation.
	Char->PlayAnimMontage(InstigatorMontage, 1.f, StartAttackMontageSectionName);
	Enemy->PlayAnimMontage(TargetMontage, 1.f, StartAttackMontageSectionName);

	// Listen for when the DealDamage tag is applied.
	if (UAbilitySystemComponent* AbilitySystemComp = Char->GetAbilitySystemComp())
	{
		AbilitySystemComp->AddGameplayEventTagContainerDelegate(
			FGameplayTagContainer{DealDamageTag},
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnAttackEnds));
	}
}

void UP15MeleeAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15MeleeAbility::OnAttackEnds(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData)
{
	EARLY_RETURN_IF(!Enemy || !DamageGameplayEffectClass)

	// Finish attack animation.
	Char->PlayAnimMontage(InstigatorMontage, 1.f, EndAttackMontageSectionName);
	Enemy->PlayAnimMontage(TargetMontage, 1.f, EndAttackMontageSectionName);

	ApplyGameplayEffectToTarget(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(Enemy.Get()),
		DamageGameplayEffectClass,
		1.f);

	//// The same effect but called from enemy's ability component.
	// if (UAbilitySystemComponent* AbilitySystemComponent = Enemy->GetAbilitySystemComp().Get())
	// {
	// 	const UGameplayEffect*             DefaultEffect = DamageGameplayEffectClass->GetDefaultObject<UGameplayEffect>();
	// 	const FGameplayEffectContextHandle Context       = MakeEffectContext(CurrentSpecHandle, CurrentActorInfo);
	// 	AbilitySystemComponent->ApplyGameplayEffectToSelf(DefaultEffect, 1.f, Context);
	// }

	Enemy = nullptr;
}
