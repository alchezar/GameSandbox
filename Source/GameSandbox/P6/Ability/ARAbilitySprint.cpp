// Copyright (C) 2023, IKinder

#include "ARAbilitySprint.h"
#include "GameFramework/CharacterMovementComponent.h"

void UARAbilitySprint::StartAction_Implementation(AActor* Instigator)
{
	Super::StartAction_Implementation(Instigator);

	UCharacterMovementComponent* MovementComp = Instigator->FindComponentByClass<UCharacterMovementComponent>();
	if (!MovementComp) return;

	if (DefaultSpeed == 0.f)
	{
		DefaultSpeed = MovementComp->MaxWalkSpeed;
	}

	MovementComp->MaxWalkSpeed = MovementComp->MaxWalkSpeed + BonusSpeed;
}

void UARAbilitySprint::StopAction_Implementation(AActor* Instigator)
{
	Super::StopAction_Implementation(Instigator);

	UCharacterMovementComponent* MovementComp = Instigator->FindComponentByClass<UCharacterMovementComponent>();
	if (!MovementComp) return;

	MovementComp->MaxWalkSpeed = DefaultSpeed;
}
