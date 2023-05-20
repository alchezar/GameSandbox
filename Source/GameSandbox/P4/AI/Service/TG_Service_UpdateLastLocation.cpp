// Copyright (C) 2023, IKinder

#include "TG_Service_UpdateLastLocation.h"
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

	const AActor* Focus = Cast<AActor>(Blackboard->GetValueAsObject(ActorKey.SelectedKeyName));
	if (!Focus || Focus->ActorHasTag(FriendTag))
	{
		Blackboard->ClearValue(LastSeenLocationKey.SelectedKeyName);
		return;
	}
	
	const FVector LastLocation = Focus->GetActorLocation();
	Blackboard->SetValueAsVector(LastSeenLocationKey.SelectedKeyName, LastLocation);

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
