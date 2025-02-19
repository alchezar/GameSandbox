// Copyright © 2025, Ivan Kinder

#include "Gameplay/Abilities/P15DeathAbility.h"

#include "AbilitySystemBlueprintLibrary.h"
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

	FTimerHandle DeathTimer;
	GetWorld()->GetTimerManager().SetTimer(DeathTimer, this, &ThisClass::EnableRagdoll, 1.f, false);
}

void UP15DeathAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const bool bReplicateEndAbility, const bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UP15DeathAbility::EnableRagdoll() const
{
	check(Char->GetCharacterMovement())
	check(Char->GetCapsuleComponent())
	check(Char->GetMesh())

	Char->GetCharacterMovement()->DisableMovement();
	Char->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Char->GetMesh()->SetSimulatePhysics(true);
	Char->GetMesh()->SetCollisionProfileName("Ragdoll");
	if (AController* Control = Char->GetController())
	{
		Control->ChangeState(NAME_Spectating);
	}
	Char->DetachFromControllerPendingDestroy();
	Char->SetLifeSpan(Lifespan);
}
