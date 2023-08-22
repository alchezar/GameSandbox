// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10AIGuardController.h"

#include "Navigation/PathFollowingComponent.h"
#include "P10/Public/Player/P10AIGuard.h"

AP10AIGuardController::AP10AIGuardController()
{
	// PrimaryActorTick.bCanEverTick = true;
}

void AP10AIGuardController::BeginPlay()
{
	Super::BeginPlay();
}

void AP10AIGuardController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10AIGuardController::OnMoveCompleted(FAIRequestID RequestID, EPathFollowingResult::Type Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (AP10AIGuard* Guard = Cast<AP10AIGuard>(GetPawn()))
	{
		Guard->MoveToTarget();
	}
}

