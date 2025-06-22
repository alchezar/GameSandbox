// Copyright © 2025, Ivan Kinder

#include "Player/AnimInstance/P17AnimInstance.h"

#include "Project17.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

void UP17AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(TryGetPawnOwner());
	RETURN_IF(!Owner,)
	Movement = Owner->GetCharacterMovement();
	RETURN_IF(!Movement,)
}

void UP17AnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	RETURN_IF(!Owner || !Movement,)
	Velocity = Movement->Velocity;
	Speed = Velocity.Size2D();
	Direction = GetMovementDirectionAngle();
	bInAir = Movement->IsFalling();
	bCrouch = Owner->bIsCrouched;
	bMoving = Velocity.SizeSquared2D() > FMath::Square(MinimalSpeed);
}

float UP17AnimInstance::GetMovementDirectionAngle() const
{
	RETURN_IF(Speed < MinimalSpeed, 0.f)

	const FVector Vector1 = Owner->GetActorForwardVector();
	const FVector Vector2 = Velocity.GetSafeNormal2D();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(Vector1 | Vector2));
	const float Sign = FMath::Sign((Vector1 ^ Vector2).Z);

	return Angle * Sign;
}
