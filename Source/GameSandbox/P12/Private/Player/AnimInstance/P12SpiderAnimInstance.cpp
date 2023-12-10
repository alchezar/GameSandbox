// Copyright (C) 2023, IKinder

#include "P12/Public/Player/AnimInstance/P12SpiderAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "P12/Public/Component/MOvement/P12SpiderMovementComponent.h"
#include "P12/Public/Player/P12SpiderPawn.h"

void UP12SpiderAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	check(TryGetPawnOwner()->IsA<AP12SpiderPawn>())
	CachedPawn = StaticCast<AP12SpiderPawn*>(TryGetPawnOwner());
	check(CachedPawn->GetMovementComponent()->IsA<UP12SpiderMovementComponent>())
	CachedPawnMovement = StaticCast<UP12SpiderMovementComponent*>(CachedPawn->GetMovementComponent());
}

void UP12SpiderAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!CachedPawn.IsValid())
	{
		return;
	}
	InputRight = CachedPawn->GetInputForward();
	InputForward = CachedPawn->GetInputRight();
	// bInAir = CachedPawn->GetMovementComponent()->IsFalling();
	bInAir = CachedPawnMovement->GetIsJump();

	RightFrontFootEffectorLocation = FVector(CachedPawn->GetIKRightFrontFootOffset(), 0.f, 0.f);
	RightRearFootEffectorLocation = FVector(CachedPawn->GetIKRightRearFootOffset(), 0.f, 0.f);
	LeftFrontFootEffectorLocation = FVector(CachedPawn->GetIKLeftFrontFootOffset(), 0.f, 0.f);
	LeftRearFootEffectorLocation = FVector(CachedPawn->GetIKLeftRearFootOffset(), 0.f, 0.f);
}
