// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P7/Public/Player/P7Character.h"

void UP7AnimInstance::NativeInitializeAnimation()
{
	Player = Cast<AP7Character>(TryGetPawnOwner());
	if (!Player) return;
	MovementComponent = Player->GetCharacterMovement();
}

void UP7AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Player || !MovementComponent) return;

	Speed = Player->GetVelocity().Size2D();
	bMoving = Speed > 0.f;
	bJump = MovementComponent->IsFalling();
	bDoubleJump = Player->GetIsDoubleJump();
	CharacterState = Player->GetCharacterState();
	Direction = GetMovementDirection();
}

float UP7AnimInstance::GetMovementDirection()
{
	if (FMath::IsNearlyZero(Speed)) return 0.f;

	const FVector MeshDirection = Player->GetActorForwardVector();
	const FVector MoveDirection = Player->GetVelocity().GetSafeNormal();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(MeshDirection, MoveDirection)));
	const float AngleSign = FMath::Sign(FVector::CrossProduct(MeshDirection, MoveDirection).Z);	
	return Angle * AngleSign;
}
