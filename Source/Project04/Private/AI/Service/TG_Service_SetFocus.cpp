// Copyright (C) 2023, IKinder

#include "AI/Service/TG_Service_SetFocus.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTG_Service_SetFocus::UTG_Service_SetFocus()
{
	NodeName = "Set Focus";
}

void UTG_Service_SetFocus::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAIController*              Controller = OwnerComp.GetAIOwner();
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard) return;

	AActor* Focus = Cast<AActor>(Blackboard->GetValueAsObject(FocusActorKey.SelectedKeyName));
	Controller->SetFocus(Focus);

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
