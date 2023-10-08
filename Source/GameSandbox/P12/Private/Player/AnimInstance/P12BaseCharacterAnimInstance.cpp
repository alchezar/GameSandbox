// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimInstance/P12BaseCharacterAnimInstance.h"

#include "P12/Public/Component/P12BaseCharacterMovementComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"

void UP12BaseCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	check(TryGetPawnOwner()->IsA<AP12BaseCharacter>())
	CachedCharacter = StaticCast<AP12BaseCharacter*>(TryGetPawnOwner());
	CachedCharacterMovement = CachedCharacter->GetBaseCharacterMovement();
	check(CachedCharacterMovement.IsValid())
}

void UP12BaseCharacterAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedCharacter.IsValid())
	{
		return;
	}
	Speed = CachedCharacter->GetVelocity().Size2D();
	Direction = GetMovementDirectionAngle();
	bInAir = CachedCharacterMovement->IsFalling();
	bCrouch = CachedCharacterMovement->IsCrouching();
}

float UP12BaseCharacterAnimInstance::GetMovementDirectionAngle() const
{
	if (!CachedCharacter.IsValid() || FMath::IsNearlyZero(Speed))
	{
		return 0.f;
	}
	const FVector ForwardVector = CachedCharacter->GetActorForwardVector();
	const FVector VelocityVector = CachedCharacter->GetVelocity().GetSafeNormal2D();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(ForwardVector | VelocityVector));
	const float Sign = FMath::Sign((ForwardVector ^ VelocityVector).Z);
	return Sign * Angle;
}
