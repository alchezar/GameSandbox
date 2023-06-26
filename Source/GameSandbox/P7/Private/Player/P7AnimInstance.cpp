// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P7/Public/Player/P7Character.h"

void UP7AnimInstance::NativeInitializeAnimation()
{
	Player = Cast<AP7Character>(TryGetPawnOwner());
	if (!Player) return;
	MovementComponent = Player->GetCharacterMovement();
}

void UP7AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Player || !MovementComponent) return;

	Speed = Player->GetVelocity().Size2D();
	bMoving = Speed > 0.f;
	bJump = MovementComponent->IsFalling();
	bDoubleJump = Player->GetIsDoubleJump();
	CharacterState = Player->GetCharacterState();
}
