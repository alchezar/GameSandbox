// Copyright (C) 2023, IKinder

#include "ER_AnimInstance.h"
#include "GameFramework/PawnMovementComponent.h"

void UER_AnimInstance::NativeInitializeAnimation()
{
	Pawn = TryGetPawnOwner();
	if (!Pawn) return;

	MovementComponent = Pawn->GetMovementComponent();
}

void UER_AnimInstance::NativeUpdateAnimation(const float DeltaSeconds)
{
	if (!Pawn || !MovementComponent) return;

	bInAir = MovementComponent->IsFalling();
	Speed  = Pawn->GetVelocity().Size();
}
