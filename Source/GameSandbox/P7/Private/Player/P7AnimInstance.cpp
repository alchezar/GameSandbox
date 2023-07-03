// Copyright (C) 2023, IKinder

#include "P7/Public/Player/P7AnimInstance.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "P7/Public/Player/P7Character.h"

void UP7AnimInstance::NativeInitializeAnimation()
{
	Character = Cast<ACharacter>(TryGetPawnOwner());
	if (!Character) return;
	MovementComponent = Character->GetCharacterMovement();
	Player = Cast<AP7Character>(Character);
	if (!Player) return;
}

void UP7AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	if (!Character || !MovementComponent) return;
	Speed = Character->GetVelocity().Size2D();
	bMoving = Speed > 0.f;
	bJump = MovementComponent->IsFalling();
	Direction = GetMovementDirection();

	if (!Player) return;
	bDoubleJump = Player->GetIsDoubleJump();
	CharacterState = Player->GetCharacterState();
}

float UP7AnimInstance::GetMovementDirection()
{
	if (FMath::IsNearlyZero(Speed)) return 0.f;

	const FVector MeshDirection = Character->GetActorForwardVector();
	const FVector MoveDirection = Character->GetVelocity().GetSafeNormal();
	const float Angle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(MeshDirection, MoveDirection)));
	const float AngleSign = FMath::Sign(FVector::CrossProduct(MeshDirection, MoveDirection).Z);	
	return Angle * AngleSign;
}
