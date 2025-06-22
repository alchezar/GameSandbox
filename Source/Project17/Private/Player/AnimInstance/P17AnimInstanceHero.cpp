// Copyright © 2025, Ivan Kinder

#include "Player/AnimInstance/P17AnimInstanceHero.h"

#include "Project17.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/P17CharacterHero.h"

void UP17AnimInstanceHero::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	OwnerHero = Cast<AP17CharacterHero>(Owner);
}

void UP17AnimInstanceHero::NativeThreadSafeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeThreadSafeUpdateAnimation(DeltaSeconds);

	RETURN_IF(!Owner || !Movement,)
	FallingSpeed = Movement->Velocity.Z;
	bInAir = Movement->IsFalling();
	bCrouch = Owner->bIsCrouched;
	bMoving = Speed > MoveThreshold;
}
