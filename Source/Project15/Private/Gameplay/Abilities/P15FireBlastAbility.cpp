// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15FireBlastAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Project15.h"
#include "Abilities/Tasks/AbilityTask_WaitTargetData.h"
#include "Gameplay/Target/P15AroundTarget.h"
#include "Player/P15Character.h"
#include "Utils/P15Notifies.h"

UP15FireBlastAbility::UP15FireBlastAbility() {}

void UP15FireBlastAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	ListenForAnimNotifies(BlastMontage.Get());
	Char->PlayAnimMontage(BlastMontage.Get(), 1.f, StartSectionName);
}

void UP15FireBlastAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	if (WaitTargetData)
	{
		WaitTargetData->ValidData.RemoveAll(this);
		WaitTargetData->Cancelled.RemoveAll(this);
		WaitTargetData = nullptr;
	}

	if (TargetActor)
	{
		TargetActor->CancelTargeting();
		TargetActor->Destroy();
		TargetActor = nullptr;
	}

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15FireBlastAbility::ListenForAnimNotifies(UAnimMontage* InMontage)
{
	for (const FAnimNotifyEvent& NotifyEvent : InMontage->Notifies)
	{
		if (auto* NotifyWindow = Cast<UP15FireBlastAnimNotifyWindow>(NotifyEvent.NotifyStateClass.Get()))
		{
			NotifyWindow->BeginDelegate.AddUObject(this, &ThisClass::OnFireBlastPullCallback);
			NotifyWindow->EndDelegate.AddUObject(this, &ThisClass::OnFireBlastPushCallback);
		}
	}
}

void UP15FireBlastAbility::OnFireBlastPullCallback(USkeletalMeshComponent* MeshComp)
{
	EARLY_RETURN_IF(!MeshComp || !Char || MeshComp != Char->GetMesh())

	// Wait target data.
	TargetActor    = GetWorld()->SpawnActor<AP15AroundTarget>(TargetActorClass);
	WaitTargetData = UAbilityTask_WaitTargetData::WaitTargetDataUsingActor(
		this,
		NAME_None,
		EGameplayTargetingConfirmation::Instant,
		TargetActor);
	check(TargetActor && WaitTargetData);
	if (WaitTargetData)
	{
		WaitTargetData->ValidData.AddDynamic(this, &ThisClass::OnDataReceivedCallback);
		WaitTargetData->Cancelled.AddDynamic(this, &ThisClass::OnDataCancelledCallback);
		WaitTargetData->ReadyForActivation();
	}
}

void UP15FireBlastAbility::OnFireBlastPushCallback(USkeletalMeshComponent* MeshComp)
{
	EARLY_RETURN_IF(!MeshComp || !Char || MeshComp != Char->GetMesh())

	// Spawn the niagara push particles.
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PushNiagara.Get(), Char->GetActorLocation() + SpawnOffset);

	// Deal damage and push targets.
	ApplyGameplayEffectToTarget(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		LastTargetData,
		TargetDamageClass,
		1.f);
	PushTargets(PushStrength, 2.f, false);

	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UP15FireBlastAbility::OnDataReceivedCallback(const FGameplayAbilityTargetDataHandle& TargetData)
{
	EARLY_RETURN_IF(TargetData.Data.IsEmpty() || !TargetData.Get(0))

	// Spawn the niagara pull particles.
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), PullNiagara.Get(), Char->GetActorLocation() - SpawnOffset);

	// Commit ability and start showing cooldown.
	CommitAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, nullptr);
	Char->OnAbilityStarted.Broadcast(GetClass());

	LastTargetData = TargetData;
	PushTargets(PullStrength, 1.f, true);
}

void UP15FireBlastAbility::OnDataCancelledCallback(const FGameplayAbilityTargetDataHandle& TargetData)
{
	Char->StopAnimMontage();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, true);
}

void UP15FireBlastAbility::PushTargets(const float Strength, const float Duration, const bool bPull)
{
	TArray<TWeakObjectPtr<AActor>> TargetActors = LastTargetData.Get(0)->GetActors();
	for (const TWeakObjectPtr Actor : TargetActors)
	{
		AP15Character* TargetChar = Cast<AP15Character>(Actor.Get());
		CONTINUE_IF(!TargetChar)

		const FVector From      = bPull ? TargetChar->GetActorLocation() : Char->GetActorLocation();
		const FVector Towards   = bPull ? Char->GetActorLocation() : TargetChar->GetActorLocation();
		const FVector Direction = (Towards - From).GetSafeNormal2D();
		TargetChar->PushCharacter(Direction, Strength, Duration);
	}
}
