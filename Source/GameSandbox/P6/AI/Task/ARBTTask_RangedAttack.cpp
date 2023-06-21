// Copyright (C) 2023, IKinder

#include "ARBTTask_RangedAttack.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "P6/Component/ARAttributesComponent.h"

UARBTTask_RangedAttack::UARBTTask_RangedAttack()
{
	NodeName = "AR_RangedAttack";
}

EBTNodeResult::Type UARBTTask_RangedAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	const AAIController* Controller = OwnerComp.GetAIOwner();	
	if (!Blackboard || !Controller) return EBTNodeResult::Failed;
	
	APawn* Pawn = Controller->GetPawn();
	if (!Pawn) return EBTNodeResult::Failed;
	
	const ACharacter* Character = Cast<ACharacter>(Pawn);
	if (!Character) return EBTNodeResult::Failed;

	const FVector SocketLocation = Character->GetMesh()->GetSocketLocation(HandSocketName);
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (!TargetActor) return EBTNodeResult::Failed;

	if (!UARAttributesComponent::GetIsActorAlive(TargetActor)) return EBTNodeResult::Failed;

	const FVector Direction = TargetActor->GetActorLocation() - SocketLocation;
	FRotator SocketRotation = Direction.Rotation();
	SocketRotation.Pitch += FMath::RandRange(0.f, MaxSpread);
	SocketRotation.Yaw += FMath::RandRange(-MaxSpread, MaxSpread);

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	Params.Instigator = Pawn;
	
	const AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SocketLocation, SocketRotation, Params);	
	return NewProjectile ? EBTNodeResult::Succeeded : EBTNodeResult::Failed;

}
