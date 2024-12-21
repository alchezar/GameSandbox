// Copyright (C) 2023, IKinder

#include "PlayerAI/Service/STU_Service_Fire.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Component/STU_WeaponComponent.h"

USTU_Service_Fire::USTU_Service_Fire()
{
	NodeName = "Fire";
}

void USTU_Service_Fire::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const AAIController*        Controller = OwnerComp.GetAIOwner();
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	const bool bHasAim = Blackboard && Blackboard->GetValueAsObject(EnemyActorKey.SelectedKeyName);
	if (Controller)
	{
		USTU_WeaponComponent* WeaponComponent = (Controller->GetPawn())->FindComponentByClass<USTU_WeaponComponent>();
		if (WeaponComponent)
		{
			bHasAim ? WeaponComponent->StartFire() : WeaponComponent->StopFire();
		}
	}
	
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}
