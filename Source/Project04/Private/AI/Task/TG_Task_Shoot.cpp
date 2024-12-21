// Copyright (C) 2023, IKinder

#include "AI/Task/TG_Task_Shoot.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Player/TG_BaseCharacter.h"
#include "Weapon/TG_Gun.h"

EBTNodeResult::Type UTG_Task_Shoot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController*              Controller = OwnerComp.GetAIOwner();
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard) return EBTNodeResult::Failed;

	AActor* Enemy = Cast<AActor>(Blackboard->GetValueAsObject(TargetKey.SelectedKeyName));
	
	if (!Enemy) return EBTNodeResult::Failed;
	Controller->SetFocus(Enemy);

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	const ATG_BaseCharacter* Character = Cast<ATG_BaseCharacter>(Pawn);
	if (!Character) return EBTNodeResult::Failed;
	ATG_Gun* Weapon = Character->GetCurrentWeapon();
	if (!Weapon) return EBTNodeResult::Failed;

	Weapon->PullTrigger();

	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UTG_Task_Shoot::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController*              Controller = OwnerComp.GetAIOwner();
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	if (!Controller || !Blackboard) return EBTNodeResult::Failed;
	
	Controller->ClearFocus(EAIFocusPriority::Gameplay);

	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	const ATG_BaseCharacter* Character = Cast<ATG_BaseCharacter>(Pawn);
	if (!Character) return EBTNodeResult::Failed;
	ATG_Gun* Weapon = Character->GetCurrentWeapon();
	if (!Weapon) return EBTNodeResult::Failed;

	Weapon->ReleaseTrigger();
	
	return EBTNodeResult::Aborted;
}
