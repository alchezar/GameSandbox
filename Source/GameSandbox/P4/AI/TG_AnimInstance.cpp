// Copyright (C) 2023, IKinder

#include "TG_AnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void UTG_AnimInstance::NativeInitializeAnimation()
{
	Pawn = TryGetPawnOwner();
	if (!Pawn) return;

	MovementComponent = Pawn->GetMovementComponent();
}

void UTG_AnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	if (!Pawn || !MovementComponent) return;

	bInAir = MovementComponent->IsFalling();
	Speed = Pawn->GetVelocity().Size();
	Direction = GetMovementDirectionAngle();
	AimRotation = (Pawn->GetBaseAimRotation() - Pawn->GetActorRotation());
	AimRotation.Normalize();
}

float UTG_AnimInstance::GetMovementDirectionAngle() const
{
	if (Pawn->GetVelocity().IsZero()) return 0.f;

	const FVector VelocityNormal = Pawn->GetVelocity().GetSafeNormal();
	const double  AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Pawn->GetActorForwardVector(), VelocityNormal)));
	const FVector CrossProduct = FVector::CrossProduct(Pawn->GetActorForwardVector(), VelocityNormal);

	return AngleBetween * FMath::Sign(CrossProduct.Z);
}
