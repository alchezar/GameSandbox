// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10AIGuardController.h"

#include "Navigation/PathFollowingComponent.h"
#include "P10/Public/Player/P10AIGuard.h"

AP10AIGuardController::AP10AIGuardController()
{ }

void AP10AIGuardController::BeginPlay()
{
	Super::BeginPlay();
}

void AP10AIGuardController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (AP10AIGuard* Guard = Cast<AP10AIGuard>(GetPawn()))
	{
		Guard->MoveToTarget();
	}
}

