// Copyright (C) 2023, IKinder

#include "AI/Service/ARBTService_CheckAttackRange.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

void UARBTService_CheckAttackRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	CheckDistance(OwnerComp);
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}

void UARBTService_CheckAttackRange::CheckDistance(UBehaviorTreeComponent& OwnerComp)
{	
	UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	const AAIController*  Controller = OwnerComp.GetAIOwner();
	if (!BlackboardComponent || !Controller) return;
	
	const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(TargetActorKey.SelectedKeyName));
	const APawn*  AIPawn	  = Controller->GetPawn();
	if (!TargetActor || !AIPawn) return;

	const float Distance       = FVector::Dist(TargetActor->GetActorLocation(), AIPawn->GetActorLocation());
	const bool  bWithinRange   = Distance < AcceptableDistance;
	const bool  bInLineOfSight = bWithinRange ? Controller->LineOfSightTo(TargetActor) : false;

	BlackboardComponent->SetValueAsBool(AttackRangeKey.SelectedKeyName, bWithinRange && bInLineOfSight);
}
