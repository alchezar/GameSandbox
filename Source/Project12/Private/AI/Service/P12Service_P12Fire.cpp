// Copyright (C) 2023, IKinder

#include "AI/Service/P12Service_P12Fire.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/P12BaseCharacter.h"

UP12Service_P12Fire::UP12Service_P12Fire()
{
	NodeName = "P12 Fire";
}

void UP12Service_P12Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const AAIController* Controller = OwnerComp.GetAIOwner();
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard)
	{
		return;
	}
	AP12BaseCharacter* BaseCharacter = Cast<AP12BaseCharacter>(Controller->GetPawn());
	if (!BaseCharacter)
	{
		return;
	}
	const AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	bool bCanShot = TargetActor != nullptr;
	if (TargetActor)
	{
		const float SquaredDistance = FVector::DistSquared(BaseCharacter->GetActorLocation(), TargetActor->GetActorLocation());
		bCanShot &= SquaredDistance <= FMath::Square(MaxFireDistance);
	}

	BaseCharacter->FireInput(bCanShot);
}
