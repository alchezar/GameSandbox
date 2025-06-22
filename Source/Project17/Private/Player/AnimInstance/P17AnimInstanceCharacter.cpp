// Copyright © 2025, Ivan Kinder

#include "Player/AnimInstance/P17AnimInstanceCharacter.h"

#include "Project17.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/P17CharacterBase.h"

void UP17AnimInstanceCharacter::NativeInitializeAnimation()
{
	Owner = Cast<ACharacter>(TryGetPawnOwner());
	RETURN_IF(!Owner,)
	Movement = Owner->GetCharacterMovement();
	RETURN_IF(!Movement,)
}

void UP17AnimInstanceCharacter::NativeThreadSafeUpdateAnimation(const float DeltaSeconds)
{
	RETURN_IF(!Owner || !Movement,)

	Speed = Owner->GetVelocity().Size2D();
	bAcceleration = Movement->GetCurrentAcceleration().SizeSquared2D() > 0.f;
}
