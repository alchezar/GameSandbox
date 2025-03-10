// Copyright © 2025, Ivan Kinder

#include "AbilitySystem/Task/P16TargetDataUnderMouse.h"

#include "AbilitySystemComponent.h"
#include "Project16.h"
#include "Player/P16PlayerController.h"

void UP16TargetDataUnderMouse::Activate()
{
	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	EARLY_RETURN_IF(!ActorInfo)

	if (ActorInfo->IsLocallyControlled())
	{
		SendMouseCursorData();
	}
	else
	{
		const FGameplayAbilitySpecHandle SpecHandle    = GetAbilitySpecHandle();
		const FPredictionKey             PredictionKey = GetActivationPredictionKey();
		// Listen to the delegate for when the target data is replicated.
		AbilitySystemComponent->AbilityTargetDataSetDelegate(SpecHandle, PredictionKey).AddUObject(this, &ThisClass::OnTargetDataReplicatedCallback);
		// If the target data is already replicated, call the delegate.
		EARLY_RETURN_IF(AbilitySystemComponent->CallReplicatedTargetDataDelegatesIfSet(SpecHandle, PredictionKey))
		// If the target data is not yet replicated, we have to wait for it.
		SetWaitingOnRemotePlayerData();
	}
}

UP16TargetDataUnderMouse* UP16TargetDataUnderMouse::CreateTargetDataUnderMouse(UGameplayAbility* OwningAbility)
{
	UP16TargetDataUnderMouse* TargetDataUnderMouse = NewAbilityTask<UP16TargetDataUnderMouse>(OwningAbility);
	return TargetDataUnderMouse;
}

void UP16TargetDataUnderMouse::OnTargetDataReplicatedCallback(const FGameplayAbilityTargetDataHandle& DataHandle, FGameplayTag GameplayTag)
{
	AbilitySystemComponent->ConsumeClientReplicatedTargetData(GetAbilitySpecHandle(), GetActivationPredictionKey());
	EARLY_RETURN_IF(!ShouldBroadcastAbilityTaskDelegates())
	ValidData.Broadcast(DataHandle);
}

void UP16TargetDataUnderMouse::SendMouseCursorData() const
{
	// Open a prediction window, to do some logic without permission from the server.
	FScopedPredictionWindow ScopedPrediction = {AbilitySystemComponent.Get()};

	const FGameplayAbilityActorInfo* ActorInfo = Ability->GetCurrentActorInfo();
	EARLY_RETURN_IF(!ActorInfo)
	const AP16PlayerController* PlayerController = Cast<AP16PlayerController>(ActorInfo->PlayerController.Get());
	EARLY_RETURN_IF(!PlayerController)
	const FHitResult LastHitResult = PlayerController->GetCursorHit();

	FGameplayAbilityTargetData_SingleTargetHit* DataPtr    = new FGameplayAbilityTargetData_SingleTargetHit{LastHitResult};
	const FGameplayAbilityTargetDataHandle      DataHandle = {DataPtr};

	AbilitySystemComponent->ServerSetReplicatedTargetData(
		GetAbilitySpecHandle(),
		GetActivationPredictionKey(),
		DataHandle,
		FGameplayTag{},
		AbilitySystemComponent->ScopedPredictionKey);

	EARLY_RETURN_IF(!ShouldBroadcastAbilityTaskDelegates())
	ValidData.Broadcast(DataHandle);
}
