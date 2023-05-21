// Copyright (C) 2023, IKinder

#include "TG_Service_UpdateLastLocation.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UTG_Service_UpdateLastLocation::UTG_Service_UpdateLastLocation()
{
	NodeName = "Update last location";
}

void UTG_Service_UpdateLastLocation::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const AAIController*  Controller = OwnerComp.GetAIOwner();
	if (!Blackboard || !Controller) return;

	const AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	if (!TargetActor)
	{
		Blackboard->ClearValue(LastSeenLocationKey.SelectedKeyName);
		return;
	}
	const FVector LastLocation = TargetActor->GetActorLocation();
	Blackboard->SetValueAsVector(LastSeenLocationKey.SelectedKeyName, LastLocation);

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
