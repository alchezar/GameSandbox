// Copyright (C) 2023, IKinder

#include "ARBTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"

UARBTTask_RangedAttack::UARBTTask_RangedAttack()
{
	NodeName = "AR_RangedAttack";
}

EBTNodeResult::Type UARBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const AAIController* Controller = OwnerComp.GetAIOwner();	
	if (!Blackboard || !Controller) return EBTNodeResult::Failed;
	const APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;;
	const ACharacter* Character = Cast<ACharacter>(Pawn);
	if (!Character) return EBTNodeResult::Failed;

	const FVector SocketLocation = Character->GetMesh()->GetSocketLocation(HandSocketName);
	const AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	const FVector Direction = TargetActor->GetActorLocation() - SocketLocation;
	const FRotator SocketRotation = Direction.Rotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	const AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SocketLocation, SocketRotation, Params);	
	return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

}
