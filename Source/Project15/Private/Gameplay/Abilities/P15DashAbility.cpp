// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15DashAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Project15.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Camera/CameraComponent.h"
#include "Player/P15Character.h"

UP15DashAbility::UP15DashAbility()
{}

void UP15DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	EARLY_RETURN_IF(!DashMontage)

	CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr);

	// Play the dash animation montage.
	if (!PlayMontageTask)
	{
		PlayMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName{}, DashMontage);
		check(PlayMontageTask)

		MontageDelegates = {
			&PlayMontageTask->OnCompleted,
			&PlayMontageTask->OnBlendOut,
			&PlayMontageTask->OnInterrupted,
			&PlayMontageTask->OnCancelled
		};

		for (FMontageWaitSimpleDelegate* Delegate : MontageDelegates)
		{
			CONTINUE_IF(!Delegate)
			Delegate->AddDynamic(this, &ThisClass::OnMontageFinishedCallback);
		}

		PlayMontageTask->Activate();
	}

	// Stop movement input and allow to dash through other pawns.
	Char->SetCollisionResponseToPawn(ECR_Overlap);

	// Apply impulse in the direction the character is facing.
	Char->PushCharacter(Char->GetPlayerEye()->GetForwardVector(), Impulse, DashMontage->GetPlayLength());

	// Wait for the dash damage gameplay event.
	if (UAbilitySystemComponent* AbilityComp = Char->GetAbilitySystemComp().Get())
	{
		AbilityComp->AddGameplayEventTagContainerDelegate(
			FGameplayTagContainer{FGameplayTag::RequestGameplayTag("p15.skill.dash.deal_damage")},
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnGameplayEventReceived));
	}
}

void UP15DashAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	// Allow movement input and stop ignoring other pawns
	Char->SetCollisionResponseToPawn(ECR_Block);

	// Unsubscribe from montage delegates and clear the task.
	for (FMontageWaitSimpleDelegate* Delegate : MontageDelegates)
	{
		CONTINUE_IF(!Delegate)
		Delegate->RemoveAll(this);
	}
	MontageDelegates.Empty();
	PlayMontageTask = nullptr;

	// Clear excluded actors.
	OverlapExclude.Empty();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15DashAbility::OnGameplayEventReceived(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData)
{
	// Remove odd const-ness from actors.
	AActor*       TargetActor     = const_cast<AActor*>(GameplayEventData->Target.Get());
	const AActor* InstigatorActor = const_cast<AActor*>(GameplayEventData->Instigator.Get());
	EARLY_RETURN_IF(!TargetActor || !InstigatorActor)

	// Ignore the target actor if it was already hit by the dash.
	EARLY_RETURN_IF(!OverlapExclude.IsEmpty() && OverlapExclude.Contains(TargetActor))
	OverlapExclude.Add(TargetActor);

	// Deal damage to the target.
	ApplyGameplayEffectToTarget(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(TargetActor),
		DamageGameplayEffectClass,
		1.f);

	// Push and stun the target.
	AP15Character* TargetCharacter = Cast<AP15Character>(TargetActor);
	EARLY_RETURN_IF(!TargetCharacter)
	const FVector PushDirection = (TargetActor->GetActorLocation() - InstigatorActor->GetActorLocation()).GetSafeNormal2D();
	TargetCharacter->PushCharacter(PushDirection, PushStrength, StunDuration);

	// Add stun particles.
	EARLY_RETURN_IF(!StunVfx.Get() || !TargetCharacter->GetMesh())
	UNiagaraComponent* SpawnedNiagara = UNiagaraFunctionLibrary::SpawnSystemAttached(
		StunVfx.Get(),
		TargetCharacter->GetMesh(),
		"head",
		FVector{20.f, 0.f, 0.f},
		FRotator{90.f, 0.f, 0.f},
		EAttachLocation::KeepRelativeOffset,
		true);
	EARLY_RETURN_IF(SpawnedNiagara)
	SpawnedNiagara->SetFloatParameter("Life", StunDuration);
}

void UP15DashAbility::OnMontageFinishedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

FP15AbilityInfo UP15DashAbility::GetAbilityInfo()
{
	FP15AbilityInfo Info = Super::GetAbilityInfo();
	if (DashMontage)
	{
		Info.SetCooldown(DashMontage->GetPlayLength());
	}
	return Info;
}
