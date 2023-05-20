// Copyright (C) 2023, IKinder

#include "TG_Decorator_WithinRange.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

bool UTG_Decorator_WithinRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController*        Controller = OwnerComp.GetAIOwner();
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard) return false;
	const APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return false;

	const FVector SelfLocation = Pawn->GetActorLocation();

	const AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor) return false;

	const FVector TargetLocation = TargetActor->GetActorLocation();
	return (SelfLocation - TargetLocation).Size() <= Range;
}
