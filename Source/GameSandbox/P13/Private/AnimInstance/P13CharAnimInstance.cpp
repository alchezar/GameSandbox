// Copyright © 2024, IKinder

#include "P13/Public/AnimInstance/P13CharAnimInstance.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "P13/Public/Character/P13TopDownCharacter.h"

void UP13CharAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	CachedCharacter = Cast<AP13TopDownCharacter>(TryGetPawnOwner());
	check(CachedCharacter.IsValid())
}

void UP13CharAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedCharacter.IsValid())
	{
		return;
	}
	Speed = CachedCharacter->GetVelocity().Size();

	// Direction = UKismetAnimationLibrary::CalculateDirection(CachedCharacter->GetCharacterMovement()->Velocity, CachedCharacter->GetActorRotation());
	Direction = GetMovementDirectionAngle();
	bInAir = CachedCharacter->GetCharacterMovement()->IsFalling();
	bCrouch = CachedCharacter->GetCharacterMovement()->IsCrouching();
	bAim = CachedCharacter->GetMovementState() == EP13MovementState::Aim;
	MovementState = CachedCharacter->GetMovementState();

	LeftLegOffset = FVector(0.f, CachedCharacter->GetIKLeftLegOffset(), 0.f);
	RightLegOffset = FVector(0.f, CachedCharacter->GetIKRightLegOffset(), 0.f);
	HipOffset = FVector(0.f, CachedCharacter->GetIKHipOffset(), 0.f);
}

float UP13CharAnimInstance::GetMovementDirectionAngle() const
{
	if (!CachedCharacter.IsValid() || FMath::IsNearlyZero(Speed))
	{
		return 0.f;
	}
	/* As our mesh is rotated 90⁰ we need to get it's right vector as forward. */
	const FVector ForwardVector = CachedCharacter->GetMesh()->GetRightVector();
	// const FVector ForwardVector = CachedCharacter->GetActorForwardVector();
	const FVector VelocityVector = CachedCharacter->GetVelocity().GetSafeNormal2D();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(ForwardVector | VelocityVector));
	const float Sign = FMath::Sign((ForwardVector ^ VelocityVector).Z);

	return Sign * Angle;
}
