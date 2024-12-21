// Copyright (C) 2023, IKinder

#include "Component/STU_CharacterMovementComponent.h"

#include "Player/STU_BaseCharacter.h"

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
