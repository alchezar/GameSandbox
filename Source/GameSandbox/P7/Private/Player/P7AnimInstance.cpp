// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P7/Public/Player/P7Character.h"
#include "P7/Public/Player/P7BaseCharacter.h"

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
	bDoubleJump = Character->GetIsDoubleJump();
	bBlock = Character->GetIsBlocked();
	CharacterState = Player ? Player->GetCharacterState() : ECS_OneHanded;
}