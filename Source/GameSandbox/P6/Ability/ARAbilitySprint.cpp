// Copyright (C) 2023, IKinder

#include "ARAbilitySprint.h"
// #include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UARAbilitySprint::UARAbilitySprint()
{}

void UARAbilitySprint::StartAbility_Implementation(AActor* Instigator)
{
	Super::StartAbility_Implementation(Instigator);

	UCharacterMovementComponent* MovementComp = Instigator->FindComponentByClass<UCharacterMovementComponent>();
	// UCharacterMovementComponent* MovementComp = Cast<ACharacter>(Instigator)->GetCharacterMovement();
	if (!MovementComp) return;

	if (DefaultSpeed == 0.f)
	{
		DefaultSpeed = MovementComp->MaxWalkSpeed;
	}

	MovementComp->MaxWalkSpeed +=  BonusSpeed;
}

void UARAbilitySprint::StopAbility_Implementation(AActor* Instigator)
{
	Super::StopAbility_Implementation(Instigator);

	UCharacterMovementComponent* MovementComp = Instigator->FindComponentByClass<UCharacterMovementComponent>();
	// UCharacterMovementComponent* MovementComp = Cast<ACharacter>(Instigator)->GetCharacterMovement();
	if (!MovementComp) return;

	MovementComp->MaxWalkSpeed = DefaultSpeed;
}
