// Copyright (C) 2023, IKinder

#include "STU_Task_NextLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameSandbox/P2/PlayerAI/STU_AIController.h"
#include "NavigationSystem.h"

USTU_Task_NextLocation::USTU_Task_NextLocation()
{
	NodeName = "NextLocation";
}

EBTNodeResult::Type USTU_Task_NextLocation::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	// return Super::ExecuteTask(OwnerComp, NodeMemory);
	const AAIController*  Controller = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (!Controller || !Blackboard) return EBTNodeResult::Failed;

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;

	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSys) return EBTNodeResult::Failed;

	FNavLocation NavLocation;
	const bool   bFound = NavSys->GetRandomReachablePointInRadius(Pawn->GetActorLocation(), Radius, OUT NavLocation);
	if (!bFound) return EBTNodeResult::Failed;

	Blackboard->SetValueAsVector(AimLocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}
