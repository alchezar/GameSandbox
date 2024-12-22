// Copyright (C) 2023, IKinder

#include "Player/P7AnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "Player/P7BaseCharacter.h"
#include "Player/P7Character.h"

void UP7AnimInstance::NativeInitializeAnimation()
{
	Character = Cast<AP7BaseCharacter>(TryGetPawnOwner());
	if (!Character) return;
	Player = Cast<AP7Character>(Character);
	MovementComponent = Character->GetCharacterMovement();
}

void UP7AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Character || !MovementComponent) return;
	Speed = Character->GetVelocity().Size2D();
	bMoving = Speed > 0.f;
	bJump = MovementComponent->IsFalling();
	Direction = Character->GetMovementDirectionAngle();
	bDoubleJump = Character->GetJumpState() == EJS_Double;
	bBlock = Character->GetIsBlocked();
	CharacterState = Player ? Player->GetCharacterState() : ECS_OneHanded;
}