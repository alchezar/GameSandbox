// Copyright (C) 2023, IKinder

#include "P12/Public/AI/Controller/P12AICharacterController.h"

#include "Navigation/PathFollowingComponent.h"
#include "P12/Public/AI/Character/P12AICharacter.h"
#include "P12/Public/Component/Actor/P12AIPatrollingComponent.h"
#include "Perception/AISense_Sight.h"

AP12AICharacterController::AP12AICharacterController()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP12AICharacterController::BeginPlay()
{
	Super::BeginPlay();

	UP12AIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetAIPatrollingComponent();
	if (!PatrollingComponent || !PatrollingComponent->GetCanPatrol())
	{
		return;
	}
	bPatrolling = true;
	const FVector& ClosestWayPoint = PatrollingComponent->SelectClosestWayPoint();
	MoveToLocation(ClosestWayPoint);
}

void AP12AICharacterController::SetPawn(APawn* InPawn)
{
	Super::SetPawn(InPawn);

	if (!InPawn)
	{
		CachedAICharacter = nullptr;
		return;
	}

	check(InPawn->IsA<AP12AICharacter>())
	CachedAICharacter = StaticCast<AP12AICharacter*>(InPawn);
}

void AP12AICharacterController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12AICharacterController::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	Super::ActorsPerceptionUpdated(UpdatedActors);

	if (!CachedAICharacter.IsValid())
	{
		return;
	}
	MoveToNextTarget();	
}

void AP12AICharacterController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	if (Result.IsFailure())
	{
		return;
	}
	MoveToNextTarget();
}

void AP12AICharacterController::MoveToNextTarget()
{
	/* Firstly try to go to the actor if had seen it. */
	AActor* ClosestActor = GetClosestSensedActor(UAISense_Sight::StaticClass());
	if (ClosestActor  && !GetIsTargetReached(ClosestActor->GetActorLocation()))
	{
		bPatrolling = false;
		MoveToActor(ClosestActor);
		return;
	}

	/* Return to the patrolling or keep it going if there are no seen actors. */
	UP12AIPatrollingComponent* PatrollingComponent = CachedAICharacter->GetAIPatrollingComponent();
	if (!PatrollingComponent || !PatrollingComponent->GetCanPatrol())
	{
		return;
	}
	const FVector& WayPoint = bPatrolling ? PatrollingComponent->SelectNextWayPoint() : PatrollingComponent->SelectClosestWayPoint();
	bPatrolling = true;
	if (GetIsTargetReached(WayPoint))
	{
		return;	
	}
	MoveToLocation(WayPoint);
}

bool AP12AICharacterController::GetIsTargetReached(const FVector& TargetLocation)
{
	const float SquaredDistance = FVector::DistSquared(CachedAICharacter->GetActorLocation(), TargetLocation); 
	return FMath::Square(TargetReachRadius) >= SquaredDistance;
}
