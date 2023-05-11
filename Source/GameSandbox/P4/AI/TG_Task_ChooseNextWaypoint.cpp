// Copyright (C) 2023, IKinder

#include "TG_Task_ChooseNextWaypoint.h"
#include "AIController.h"
#include "TG_PatrolComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/TargetPoint.h"

EBTNodeResult::Type UTG_Task_ChooseNextWaypoint::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (!OwnerComp.GetAIOwner() || !OwnerComp.GetAIOwner()->GetPawn()) return EBTNodeResult::Failed;

	const AAIController*       Controller = OwnerComp.GetAIOwner();
	const APawn*               Pawn = Controller->GetPawn();
	const UTG_PatrolComponent* Patrol = Pawn->FindComponentByClass<UTG_PatrolComponent>();
	if (!Patrol) return EBTNodeResult::Failed;

	TArray<AActor*> WayPoints;
	for (const auto Point : Patrol->GetPatrolPoints())
	{
		WayPoints.Add(Cast<AActor>(Point));
	}
	if (WayPoints.IsEmpty()) return EBTNodeResult::Failed;

	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	const int32 RandIndex = FMath::RandRange(0, WayPoints.Num() - 1);
	const int32 CurrentIndex = Blackboard->GetValueAsInt(IndexKey.SelectedKeyName);
	const int32 Index = (RandIndex == CurrentIndex) ? (CurrentIndex + 1) % WayPoints.Num() : RandIndex;
	Blackboard->SetValueAsInt(IndexKey.SelectedKeyName, Index);
	Blackboard->SetValueAsObject(WaypointKey.SelectedKeyName, WayPoints[Index]);

	return EBTNodeResult::Succeeded;
}
