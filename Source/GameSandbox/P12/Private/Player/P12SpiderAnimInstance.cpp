// Copyright (C) 2023, IKinder

#include "P12/Public/Player/P12SpiderAnimInstance.h"

#include "GameFramework/PawnMovementComponent.h"
#include "P12/Public/Player/P12SpiderPawn.h"

void UP12SpiderAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	check(TryGetPawnOwner()->IsA<AP12SpiderPawn>())
	CachedPawn = StaticCast<AP12SpiderPawn*>(TryGetPawnOwner());
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
	bInAir = CachedPawn->GetMovementComponent()->IsFalling();
}
