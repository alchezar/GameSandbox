// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15DeathAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
#include "Project15.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Gameplay/Effects/P15DeathEffect.h"
#include "Player/P15Character.h"

UP15DeathAbility::UP15DeathAbility()
{}

void UP15DeathAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);

	if (!CommitAbility(Handle, ActorInfo, ActivationInfo, nullptr))
	{
		EndAbility(Handle, ActorInfo, ActivationInfo, false, true);
	}

	FTimerHandle         DeathTimer;
	const FTimerDelegate DeathDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::EnableRagdoll, Cast<ACharacter>(ActorInfo->OwnerActor));
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, DeathDelegate, 1.f, false);
}

void UP15DeathAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15DeathAbility::EnableRagdoll(ACharacter* InChar) const
{
	EARLY_RETURN_IF(!InChar)
	check(InChar->GetCharacterMovement())
	check(InChar->GetCapsuleComponent())
	check(InChar->GetMesh())

	InChar->GetCharacterMovement()->DisableMovement();
	InChar->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	InChar->GetMesh()->SetSimulatePhysics(true);
	InChar->GetMesh()->SetCollisionProfileName("Ragdoll");
	if (AController* Control = InChar->GetController())
	{
		Control->ChangeState(NAME_Spectating);
	}
	InChar->DetachFromControllerPendingDestroy();
	InChar->SetLifeSpan(Lifespan);
}
