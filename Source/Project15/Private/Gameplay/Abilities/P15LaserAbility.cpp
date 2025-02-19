// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15LaserAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Project15.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Player/P15Character.h"

UP15LaserAbility::UP15LaserAbility()
{}

void UP15LaserAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr);

	// Start Anim montage.
	LaserMontageTask = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, FName{}, LaserMontage, 1.f, StartSection);
	check(LaserMontageTask)

	LaserMontageTask->Activate();

	// Spawn the laser actor.
	GetWorld()->GetTimerManager().SetTimer(SpawnTimer, this, &ThisClass::SpawnLaserActor, SpawnDelay);

	// Listen for when the DealDamage tag is applied.
	if (UAbilitySystemComponent* AbilityComp = Char->GetAbilitySystemComp())
	{
		AbilityComp->AddGameplayEventTagContainerDelegate(
			FGameplayTagContainer{DamageTag},
			FGameplayEventTagMulticastDelegate::FDelegate::CreateUObject(this, &ThisClass::OnDamageTagReceivedCallback));
	}
}

void UP15LaserAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	// Remove mana drain tag.
	if (UAbilitySystemComponent* AbilityComp = ActorInfo->AbilitySystemComponent.Get())
	{
		AbilityComp->RemoveActiveEffectsWithTags(FGameplayTagContainer{ManaDrainTag});

		//// Another approach as in BP_RemoveGameplayEffectFromOwnerWithAssetTags method.
		// FActiveGameplayEffectsContainer ActiveGameplayEffectsContainer = AbilityComp->GetActiveGameplayEffects();
		// const FGameplayEffectQuery Query = FGameplayEffectQuery::MakeQuery_MatchAllEffectTags(FGameplayTagContainer{ManaDrainTag});
		// AbilityComp->RemoveActiveEffects(Query);
	}

	// Destroy laser actor and stop using controller yaw rotation.
	LaserActor->Destroy();
	Char->bUseControllerRotationYaw = false;
	Char->StopAnimMontage();

	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15LaserAbility::OnDamageTagReceivedCallback(FGameplayTag Tag, const FGameplayEventData* Payload)
{
	// Stop if no mana.
	if (!GetIsCostStillAvailable())
	{
		EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, true, false);
		return;
	}

	// Apply damage.
	const FGameplayAbilityTargetDataHandle& TargetData = Payload->TargetData;
	ApplyGameplayEffectToTarget(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, Payload->TargetData, DamageEffect, 1.f);

	// Push targets in opposite directions.
	for (AActor* Target : GetTargetActors(TargetData))
	{
		AP15Character*         TargetChar = Cast<AP15Character>(Target);
		const USceneComponent* SceneComp  = Cast<USceneComponent>(Payload->OptionalObject);
		CONTINUE_IF(!TargetChar || !SceneComp)

		const FVector Direction = (Target->GetActorLocation() - SceneComp->GetComponentLocation()).GetSafeNormal2D();

		TargetChar->PushCharacter(Direction, PushStrength, Payload->EventMagnitude);
	}
}

void UP15LaserAbility::SpawnLaserActor()
{
	EARLY_RETURN_IF(!LaserActorClass)
	LaserActor = GetWorld()->SpawnActor(LaserActorClass);
	EARLY_RETURN_IF(!LaserActor)
	LaserActor->AttachToComponent(Char->GetMesh(), {EAttachmentRule::SnapToTarget, true}, AttachSocketName);
	LaserActor->SetInstigator(Char.Get());

	// Use controller rotation Yaw.
	Char->bUseControllerRotationYaw = true;
}

TArray<AActor*> UP15LaserAbility::GetTargetActors(const FGameplayAbilityTargetDataHandle& TargetData) const
{
	TArray<AActor*> Result = {};
	EARLY_RETURN_VALUE_IF(TargetData.Data.IsEmpty() || !TargetData.Get(0), Result)

	Algo::TransformIf(TargetData.Get(0)->GetActors(), Result,
		[](const TWeakObjectPtr<AActor> WeakActor) -> bool { return WeakActor.IsValid(); },
		[](const TWeakObjectPtr<AActor> WeakActor) -> AActor* { return WeakActor.Get(); });

	return Result;
}
