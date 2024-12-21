// Copyright (C) 2023, IKinder

#include "AI/Task/TG_Task_ClearBlackboardValue.h"

#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTG_Task_ClearBlackboardValue::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Blackboard) return EBTNodeResult::Failed;

	Blackboard->ClearValue(KeyToClear.SelectedKeyName);
	return EBTNodeResult::Succeeded;
}
