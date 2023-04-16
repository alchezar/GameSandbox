// Copyright (C) 2023, IKinder

#include "STU_Service_FindEnemy.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/STU_AIPerceptionComponent.h"

USTU_Service_FindEnemy::USTU_Service_FindEnemy()
{
	NodeName = "Find Enemy";
}

void USTU_Service_FindEnemy::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (Blackboard)
	{
		const AAIController* Controller   = OwnerComp.GetAIOwner();
		const auto*          AIPerception = Controller->FindComponentByClass<USTU_AIPerceptionComponent>();
		if (AIPerception)
		{
			Blackboard->SetValueAsObject(EnemyActorKey.SelectedKeyName, AIPerception->GetClosestEnemy());
		}
	}
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
