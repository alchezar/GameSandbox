// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15GroundBlastAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "Project15.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Player/P15Character.h"

UP15GroundBlastAbility::UP15GroundBlastAbility()
{}

void UP15GroundBlastAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	Char->PlayAnimMontage(BlastMontage, 1.f, StartSection);
}

void UP15GroundBlastAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15GroundBlastAbility::OnValidDataReceivedCallback(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Super::OnValidDataReceivedCallback(TargetData);

	// Play montage and apply damage.
	Char->PlayAnimMontage(BlastMontage, 1.f, EndSection);
	ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, TargetData, DamageEffectClass, 1.f);

	// Push targets up.
	for (TWeakObjectPtr Target : TargetData.Data[0]->GetActors())
	{
		AP15Character* TargetCharacter = Cast<AP15Character>(Target);
		CONTINUE_IF(!TargetCharacter)
		TargetCharacter->PushCharacter(FVector::UpVector, PushStrength);
	}

	// Spawn particles at the target location.
	if (Niagara)
	{
		const FVector TargetLocation = TargetData.Data[1]->GetEndPoint();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Niagara.Get(), TargetLocation);
	}

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UP15GroundBlastAbility::OnValidDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Char->StopAnimMontage();
	Super::OnValidDataCancelledCallback(TargetData);
}
