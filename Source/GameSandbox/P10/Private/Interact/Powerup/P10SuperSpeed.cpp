// Copyright (C) 2023, IKinder

#include "P10/Public/Interact/Powerup/P10SuperSpeed.h"

#include "GameFramework/CharacterMovementComponent.h"


AP10SuperSpeed::AP10SuperSpeed()
{
}

void AP10SuperSpeed::OnActivated(AActor* Target)
{
	Super::OnActivated(Target);
	if (!Target) return;
	
	CharMovement = Target->FindComponentByClass<UCharacterMovementComponent>();
	if (!CharMovement) return;

	DefaultMaxWalkSpeed = CharMovement->MaxWalkSpeed;
	CharMovement->MaxWalkSpeed = DefaultMaxWalkSpeed * 2.f;
}

void AP10SuperSpeed::OnExpired()
{
	if (CharMovement)
	{
		CharMovement->MaxWalkSpeed = DefaultMaxWalkSpeed;
	}	
	Super::OnExpired();
}
