// Copyright (C) 2023, IKinder

#include "TG_Task_FocusAtActor.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

EBTNodeResult::Type UTG_Task_FocusAtActor::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController*              Controller = OwnerComp.GetAIOwner();
	if (!Controller || !Blackboard) return EBTNodeResult::Failed;
	
	AActor* FocusedActor = Cast<AActor>(Blackboard->GetValueAsObject(FocusKey.SelectedKeyName));
	if (!FocusedActor) return EBTNodeResult::Failed;
	
	Controller->SetFocus(FocusedActor);
	return EBTNodeResult::Succeeded;
}
