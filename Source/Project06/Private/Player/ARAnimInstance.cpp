// Copyright (C) 2023, IKinder

#include "Player/ARAnimInstance.h"

#include "Component/ARAttributesComponent.h"
#include "GameFramework/PawnMovementComponent.h"

void UARAnimInstance::NativeInitializeAnimation()
{
	Pawn = TryGetPawnOwner();
	if (!Pawn) return;

	MovementComponent = Pawn->GetMovementComponent();
	AttributesComponent = UARAttributesComponent::GetAttributes(Pawn);	
}

void UARAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!AttributesComponent) return;
	
	bAlive = AttributesComponent ? AttributesComponent->GetIsAlive() : false;
	bJump = MovementComponent->IsFalling();

	FVector Velocity = Pawn->GetVelocity();
	Velocity.Z = 0.f;
	Speed = Velocity.Size();
	
	const float DirectionAngle = GetMovementDirectionAngle();
	Direction = FMath::IsNearlyZero(DirectionAngle) ? Direction : DirectionAngle;	
}

float UARAnimInstance::GetMovementDirectionAngle()
{
	if (Pawn->GetVelocity().IsZero()) return 0.f;

	const FVector MeshDirection   = Pawn->GetActorForwardVector();
	const FVector MovingDirection = Pawn->GetVelocity().GetSafeNormal();

	const float AngleBetween   = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(MeshDirection, MovingDirection)));
	const FVector CrossProduct = FVector::CrossProduct(MeshDirection, MovingDirection);

	return AngleBetween * FMath::Sign(CrossProduct.Z);
}
