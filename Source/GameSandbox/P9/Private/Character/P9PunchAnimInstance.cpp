// Copyright (C) 2023, IKinder

#include "P9/Public/Character/P9PunchAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "P9/Public/Character/P9PunchCharacter.h"

void UP9PunchAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Owner = Cast<AP9PunchCharacter>(TryGetPawnOwner());
	if (!Owner) return;
	MovementComponent = Owner->GetCharacterMovement();
}

void UP9PunchAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!Owner || !MovementComponent) return;
	
	Speed = Owner->GetVelocity().Size2D();
	Direction = GetMovementDirectionAngle();
	bInAir = MovementComponent->IsFalling();
	bArmed  = Owner->GetCharState() >= EP9CharState::ARMED ;
	bMoving = Speed > 10.f;
	bCrouching = MovementComponent->IsCrouching();
}

float UP9PunchAnimInstance::GetMovementDirectionAngle() const
{
	if (FMath::IsNearlyZero(Owner->GetVelocity().Size2D())) return 0.f;
	const FVector Vector1 = Owner->GetActorForwardVector();
	const FVector Vector2 = Owner->GetVelocity().GetSafeNormal();
	const float Sign = FMath::Sign(Vector1.Cross(Vector2).Z);
	return Sign * FMath::RadiansToDegrees(FMath::Acos(Vector1.Dot(Vector2)));
}
