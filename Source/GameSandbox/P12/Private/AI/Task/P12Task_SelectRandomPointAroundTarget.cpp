// Copyright (C) 2023, IKinder

#include "P12/Public/AI/Task/P12Task_SelectRandomPointAroundTarget.h"

#include "AIController.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"

UP12Task_SelectRandomPointAroundTarget::UP12Task_SelectRandomPointAroundTarget()
{
	NodeName = "Random point around target";
}

EBTNodeResult::Type UP12Task_SelectRandomPointAroundTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const AAIController* AIController = OwnerComp.GetAIOwner();
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!AIController || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}
	APawn* Pawn = AIController->GetPawn();
	if (!Pawn)
	{
		return EBTNodeResult::Failed;
	}
	const UNavigationSystemV1* NavSys = UNavigationSystemV1::GetCurrent(Pawn);
	if (!NavSys)
	{
		return EBTNodeResult::Failed;
	}
	const AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor)
	{
		return EBTNodeResult::Failed;
	}
	const FVector& TargetLocation = TargetActor->GetActorLocation();
	FNavLocation NavLocation;
	const bool bReached = NavSys->GetRandomReachablePointInRadius(TargetLocation, Radius, NavLocation);
	if (!bReached)
	{
		return EBTNodeResult::Failed;
	}
	Blackboard->SetValueAsVector(LocationKey.SelectedKeyName, NavLocation.Location);
	return EBTNodeResult::Succeeded;
}
