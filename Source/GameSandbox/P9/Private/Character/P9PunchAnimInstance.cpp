// Copyright (C) 2023, IKinder

#include "P9/Public/Character/P9PunchAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "P9/Public/Character/P9PunchCharacter.h"

void UP9PunchAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	PunchCharacter = Cast<AP9PunchCharacter>(TryGetPawnOwner());
	if (!PunchCharacter) return;
	MovementComponent = PunchCharacter->GetCharacterMovement();
}

void UP9PunchAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!PunchCharacter || !MovementComponent) return;
	
	Speed = PunchCharacter->GetVelocity().Size2D();
	Direction = PunchCharacter->GetMovementDirectionAngle();
	bInAir = MovementComponent->IsFalling();
}
