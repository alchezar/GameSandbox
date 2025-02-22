// Copyright (C) 2023, IKinder

#include "Player/P10AnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "Player/P10Character.h"

void UP10AnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Pawn = TryGetPawnOwner();
	if (!Pawn) return;
	MovementComponent = Pawn->GetMovementComponent();

	PPCharacter = Cast<AP10Character>(Pawn);
}

void UP10AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	if (!Pawn || !MovementComponent) return; 
	
	bInAir = MovementComponent->IsFalling();
	bCrouch = MovementComponent->IsCrouching();
	Speed = Pawn->GetVelocity().Size2D();
	AimRotation = Pawn->GetBaseAimRotation() - Pawn->GetActorRotation();
	AimRotation.Normalize();
	Direction = GetMovementDirectionAngle();

	if (!PPCharacter) return; 
	bShooting = PPCharacter->GetIsShooting();
	bAiming = PPCharacter->GetIsAiming();
}

float UP10AnimInstance::GetMovementDirectionAngle() const
{
	const FVector VelocityNormal = Pawn->GetVelocity().GetSafeNormal();
	const float AngleBetween = FMath::RadiansToDegrees(FMath::Acos(Pawn->GetActorForwardVector().Dot(VelocityNormal)));
	const FVector CrossProduct = Pawn->GetActorForwardVector().Cross(VelocityNormal);

	return FMath::Sign(CrossProduct.Z) * AngleBetween;
}