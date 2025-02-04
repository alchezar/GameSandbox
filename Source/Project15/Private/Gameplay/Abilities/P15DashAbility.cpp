// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15DashAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project15.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/P15Character.h"

UP15DashAbility::UP15DashAbility()
{}

void UP15DashAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	AP15Character* Char = Cast<AP15Character>(ActorInfo->OwnerActor.Get());
	EARLY_RETURN_IF(!Char)
	UCharacterMovementComponent* CharacterMovementComp = Char->GetCharacterMovement();
	EARLY_RETURN_IF(!CharacterMovementComp)
	EARLY_RETURN_IF(!DashMontage)

	CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr);

	// Stop movement input and allow to dash through other pawns.
	Char->SetAllowMoving(false);
	Char->SetCollisionResponseToPawn(ECR_Overlap);

	// Reduce ground friction while dashing.
	CurrentGroundFriction  = &CharacterMovementComp->GroundFriction;
	DefaultGroundFriction  = *CurrentGroundFriction;
	*CurrentGroundFriction = 0.f;

	// Apply impulse in the direction the character is facing.
	const FVector ForwardDirection = Char->GetPlayerEye()->GetForwardVector();
	CharacterMovementComp->AddImpulse(ForwardDirection * Impulse, true);

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
	AP15Character* Char = Cast<AP15Character>(ActorInfo->OwnerActor);
	EARLY_RETURN_IF(!Char)
	Char->SetAllowMoving(true);
	Char->SetCollisionResponseToPawn(ECR_Block);

	// Restore ground friction.
	if (CurrentGroundFriction)
	{
		*CurrentGroundFriction = DefaultGroundFriction;
	}

	// Unsubscribe from montage delegates and clear the task.
	for (FMontageWaitSimpleDelegate* Delegate : MontageDelegates)
	{
		CONTINUE_IF(!Delegate)
		Delegate->RemoveAll(this);
	}
	PlayMontageTask = nullptr;

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15DashAbility::OnGameplayEventReceived(FGameplayTag GameplayTag, const FGameplayEventData* GameplayEventData)
{
	ApplyGameplayEffectToTarget(
		CurrentSpecHandle,
		CurrentActorInfo,
		CurrentActivationInfo,
		UAbilitySystemBlueprintLibrary::AbilityTargetDataFromActor(const_cast<AActor*>(GameplayEventData->Target.Get())),
		DamageGameplayEffectClass,
		1.f);
}

void UP15DashAbility::OnMontageFinishedCallback()
{
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}
