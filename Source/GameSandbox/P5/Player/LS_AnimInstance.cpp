// Copyright (C) 2023, IKinder

#include "LS_AnimInstance.h"
#include "LS_BaseCharacter.h"
#include "LS_PlayerCharacter.h"
#include "GameFramework/PawnMovementComponent.h"

void ULS_AnimInstance::NativeInitializeAnimation()
{
	Pawn = TryGetPawnOwner();
	if (!Pawn) return;

	MovementComponent = Pawn->GetMovementComponent();
}

void ULS_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Pawn || !MovementComponent) return;

	Speed = Pawn->GetVelocity().Size();
	Direction = GetMovementDirectionAngle();
	bJump = MovementComponent->IsFalling();

	if (const auto Character = Cast<ALS_BaseCharacter>(Pawn))
	{
		bDead = Character->GetIsDead();
		bDoubleJump = Character->GetIsDoubleJump();
	}
	if (const auto Player = Cast<ALS_PlayerCharacter>(Pawn))
	{
		bTurnLeft = Player->GetTurnLeft();
		bTurnRight = Player->GetTurnRight();
	}
}

float ULS_AnimInstance::GetMovementDirectionAngle()
{
	if (Pawn->GetVelocity().IsZero()) return 0.f;

	const FVector VelocityNormal = Pawn->GetVelocity().GetSafeNormal();
	const double AngleBetween = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Pawn->GetActorForwardVector(), VelocityNormal)));
	const FVector CrossProduct = FVector::CrossProduct(Pawn->GetActorForwardVector(), VelocityNormal);

	return AngleBetween * FMath::Sign(CrossProduct.Z);
}