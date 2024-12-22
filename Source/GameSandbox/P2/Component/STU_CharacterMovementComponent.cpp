// Copyright (C) 2023, IKinder

#include "STU_CharacterMovementComponent.h"
#include "GameSandbox/P2/Player/STU_BaseCharacter.h"

USTU_CharacterMovementComponent::USTU_CharacterMovementComponent()
{
	GetNavAgentPropertiesRef().bCanCrouch = true;
}

float USTU_CharacterMovementComponent::GetMaxSpeed() const
{
	const float MaxSpeed = Super::GetMaxSpeed();
	const auto* Player   = Cast<ASTU_BaseCharacter>(GetCharacterOwner());

	return (Player && Player->GetIsRunning() && !IsCrouching()) ? MaxSpeed * RunModifier : MaxSpeed;
}
