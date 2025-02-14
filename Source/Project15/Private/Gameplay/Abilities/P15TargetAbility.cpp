// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15TargetAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Gameplay/Target/P15GroundSelectTarget.h"
#include "Player/P15Character.h"

UP15TargetAbility::UP15TargetAbility()
{
	TargetActorClass = AP15GroundSelectTarget::StaticClass();
}

void UP15TargetAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	// Check if we have enough mana.
	if (!GetIsCostStillAvailable())
	{
		CancelAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false);
	}

	// For some reason WaitTargetData(...) method doesn't work, so we need to spawn the target actor by ourselves.
	TargetActor    = GetWorld()->SpawnActor<AP15GroundSelectTarget>(TargetActorClass);
	WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::UserConfirmed,
		TargetActor);

	check(TargetActor && WaitTargetData);

	WaitTargetData->ValidData.AddDynamic(this, &ThisClass::OnValidDataReceivedCallback);
	WaitTargetData->Cancelled.AddDynamic(this, &ThisClass::OnValidDataCancelledCallback);
	WaitTargetData->ReadyForActivation();
	TargetActor->StartTargeting(this);

	// Set targeting state.
	Char->SetTargetingState(true);
}

void UP15TargetAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	if (WaitTargetData)
	{
		WaitTargetData->ValidData.RemoveAll(this);
		WaitTargetData = nullptr;
	}
	TargetActor->Destroy();
	TargetActor = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15TargetAbility::OnValidDataReceivedCallback(const FGameplayAbilityTargetDataHandle& TargetData)
{
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo);
	Char->OnAbilityStarted.Broadcast(GetClass());
	Char->SetTargetingState(false);
}

void UP15TargetAbility::OnValidDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Char->SetTargetingState(false);
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}
