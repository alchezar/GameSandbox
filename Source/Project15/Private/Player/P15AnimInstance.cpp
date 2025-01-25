// Copyright © 2025, Ivan Kinder

#include "Player/P15AnimInstance.h"

#include "Project15.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Player/P15Character.h"

void UP15AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<AP15Character>(TryGetPawnOwner());
	if (!Owner.Get())
	{
		return;
	}
	Movement = Owner->GetCharacterMovement();
	check(Movement.Get())
}

void UP15AnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner.Get() || !Movement.Get())
	{
		return;
	}

	Velocity  = Movement->Velocity;
	Speed     = Velocity.Size2D();
	Direction = GetMovementDirectionAngle();
	bInAir    = Movement->IsFalling();
	bMoving   = Speed > P15::MinimalSpeed;
	bCrouch   = Owner->bIsCrouched;
}

float UP15AnimInstance::GetMovementDirectionAngle() const
{
	if (Speed < P15::MinimalSpeed)
	{
		return 0;
	}

	const FVector Vector1 = Owner->GetActorForwardVector();
	const FVector Vector2 = Velocity.GetSafeNormal2D();
	const float   Angle   = FMath::RadiansToDegrees(FMath::Acos(Vector1 | Vector2));
	const float   Sign    = FMath::Sign((Vector1 ^ Vector2).Z);

	return Angle * Sign;
}
