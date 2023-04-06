// Copyright (C) 2023, IKinder

#include "STU_CharacterMovementComponent.h"
#include "GameSandbox/P2/Player/STU_BaseCharacter.h"

float USTU_CharacterMovementComponent::GetMaxSpeed() const
{
	const float               MaxSpeed = Super::GetMaxSpeed();
	const ASTU_BaseCharacter* Player   = Cast<ASTU_BaseCharacter>(GetCharacterOwner());

	return (Player && Player->GetIsRunning() && !IsCrouching()) ? MaxSpeed * RunModifier : MaxSpeed;
}
