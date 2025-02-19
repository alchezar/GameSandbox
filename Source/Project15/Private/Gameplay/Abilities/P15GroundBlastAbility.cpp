// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15GroundBlastAbility.h"

#include "NiagaraFunctionLibrary.h"
#include "Project15.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Player/P15Character.h"
#include "Utils/P15Notifies.h"

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
	// Listen for the anim notify in the end section, where we can use the target data.
	ListenForAnimNotifies(BlastMontage);
	Data = TargetData;
	Char->PlayAnimMontage(BlastMontage, 1.f, EndSection);
}

void UP15GroundBlastAbility::OnValidDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Char->StopAnimMontage();
	Super::OnValidDataCancelledCallback(TargetData);
}

void UP15GroundBlastAbility::ListenForAnimNotifies(UAnimMontage* InMontage)
{
	for (const FAnimNotifyEvent& NotifyEvent : InMontage->Notifies)
	{
		if (UP15GroundBlastAnimNotifyWindow* GroundBlastNotify = Cast<UP15GroundBlastAnimNotifyWindow>(NotifyEvent.NotifyStateClass))
		{
			GroundBlastNotify->BeginDelegate.AddUObject(this, &ThisClass::OnGroundBlastBeginCallback);
			GroundBlastNotify->EndDelegate.AddUObject(this, &ThisClass::OnGroundBlastEndCallback);
		}
	}
}

void UP15GroundBlastAbility::OnGroundBlastBeginCallback(USkeletalMeshComponent* MeshComp)
{
	EARLY_RETURN_IF(!MeshComp || !Char || MeshComp != Char->GetMesh())

	// We need to call the method from the parent class here, for correct execution order.
	Super::OnValidDataReceivedCallback(Data);

	ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Data, DamageEffectClass, 1.f);

	// Push targets up.
	for (TWeakObjectPtr Target : Data.Data[0]->GetActors())
	{
		AP15Character* TargetCharacter = Cast<AP15Character>(Target);
		CONTINUE_IF(!TargetCharacter)
		TargetCharacter->PushCharacter(FVector::UpVector, PushStrength);
	}

	// Spawn particles at the target location.
	if (Niagara)
	{
		const FVector TargetLocation = Data.Data[1]->GetEndPoint();
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Niagara.Get(), TargetLocation);
	}
}

void UP15GroundBlastAbility::OnGroundBlastEndCallback(USkeletalMeshComponent* MeshComp)
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
