// Copyright © 2025, Ivan Kinder

#include "Public/Character/AnimInstance/P17AnimInstanceCharacter.h"

#include "Project17.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Public/Character/P17CharacterBase.h"

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
	Direction = GetMovementDirectionAngle();
}

float UP17AnimInstanceCharacter::GetMovementDirectionAngle() const
{
	RETURN_IF(Speed < 10.f, 0.f)

	const FVector Vector1 = Owner->GetActorForwardVector();
	const FVector Vector2 = Owner->GetVelocity().GetSafeNormal2D();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(Vector1 | Vector2));
	const float Sign = FMath::Sign((Vector1 ^ Vector2).Z);

	return Angle * Sign;
}
