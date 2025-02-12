// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15GroundBlastAbility.h"

#include "Project15.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Gameplay/Target/P15GroundSelectTarget.h"

UP15GroundBlastAbility::UP15GroundBlastAbility()
{
	TargetActorClass = AP15GroundSelectTarget::StaticClass();
}

void UP15GroundBlastAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr);

	WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetData(this, FName{}, EGameplayTargetingConfirmation::Instant, TargetActorClass);
	EARLY_RETURN_IF(!WaitTargetData)
	WaitTargetData->ValidData.AddDynamic(this, &ThisClass::OnValidDataReceivedCallback);
	WaitTargetData->Activate();
}

void UP15GroundBlastAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	if (WaitTargetData)
	{
		WaitTargetData->ValidData.RemoveAll(this);
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15GroundBlastAbility::OnValidDataReceivedCallback(const FGameplayAbilityTargetDataHandle& TargetData)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Cyan, "Valid data received!");
}
