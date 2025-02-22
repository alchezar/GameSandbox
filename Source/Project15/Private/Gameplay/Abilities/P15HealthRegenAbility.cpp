// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15HealthRegenAbility.h"

#include "AbilitySystemComponent.h"
#include "Project15.h"
#include "Player/P15Character.h"

UP15HealthRegenAbility::UP15HealthRegenAbility()
{
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag("p15.health.full"));
}

void UP15HealthRegenAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Play a random montage section.
	EARLY_RETURN_IF(!RegenMontage)
	const int32 RandomSectionIndex  = FMath::Rand32() % RegenMontage->GetNumSections();
	const FName RandomSectionName   = RegenMontage->GetSectionName(RandomSectionIndex);
	const float RandomSectionLength = RegenMontage->GetSectionLength(RandomSectionIndex);
	Char->PlayAnimMontage(RegenMontage.Get(), 1.f, RandomSectionName);
	Char->Stun(RandomSectionLength);

	FTimerHandle         RegenTimer;
	const FTimerDelegate TimerDelegate = FTimerDelegate::CreateWeakLambda(this, [this]() -> void
	{
		CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
	});
	GetWorld()->GetTimerManager().SetTimer(RegenTimer, TimerDelegate, RegenMontage->GetSectionLength(RandomSectionIndex), false);
}

void UP15HealthRegenAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	// One way to apply the gameplay effect.
	UAbilitySystemComponent* AbilitySystemComp = Char->GetAbilitySystemComp().Get();
	EARLY_RETURN_IF(!AbilitySystemComp || !RegenEffectClass)
	FGameplayEffectContextHandle EffectContext = AbilitySystemComp->MakeEffectContext();
	EffectContext.AddSourceObject(Char);
	const FGameplayEffectSpecHandle EffectSpecHandle = AbilitySystemComp->MakeOutgoingSpec(RegenEffectClass, 1.f, MoveTemp(EffectContext));
	EARLY_RETURN_IF(!EffectSpecHandle.IsValid())
	AbilitySystemComp->ApplyGameplayEffectSpecToSelf(*EffectSpecHandle.Data.Get());

	//// Another way to apply the gameplay effect.
	// ApplyGameplayEffectToOwner(
	// 	Handle,
	// 	ActorInfo,
	// 	ActivationInfo,
	// 	RegenEffectClass->GetDefaultObject<UGameplayEffect>(),
	// 	1.f);

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}
