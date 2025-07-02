// Copyright © 2025, Ivan Kinder

#include "AI/P17BTSOrientToTarget.h"

#include "AIController.h"
#include "Project17.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BlackboardData.h"

UP17BTSOrientToTarget::UP17BTSOrientToTarget()
{
	NodeName = "BTS17 Native Orient To Target Actor";
	INIT_SERVICE_NODE_NOTIFY_FLAGS();
	Interval = 0.f;
	RandomDeviation = 0.f;

	InTargetActorKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetActorKey), AActor::StaticClass());
}

void UP17BTSOrientToTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	RETURN_IF(!BlackboardAsset,)
	InTargetActorKey.ResolveSelectedKey(*BlackboardAsset);
}

FString UP17BTSOrientToTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetActorKey.SelectedKeyName.ToString();
	return FString::Printf(L"Orient rotation to %s Key %s", *KeyDescription, *GetStaticServiceDescription());
}

void UP17BTSOrientToTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, const float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
	RETURN_IF(!BlackboardComponent,)
	const AActor* TargetActor = Cast<AActor>(BlackboardComponent->GetValueAsObject(InTargetActorKey.SelectedKeyName));
	RETURN_IF(!TargetActor,)

	const AAIController* OwnerController = OwnerComp.GetAIOwner();
	RETURN_IF(!OwnerController,)
	APawn* OwnerPawn = OwnerController->GetPawn();
	RETURN_IF(!OwnerPawn,)

	const FVector LookAtDirection = TargetActor->GetActorLocation() - OwnerPawn->GetActorLocation();
	const FRotator LookAtRotation = FRotationMatrix::MakeFromX(LookAtDirection).Rotator();
	const FRotator TargetRotation = FMath::RInterpTo(OwnerPawn->GetActorRotation(), LookAtRotation, DeltaSeconds, RotationInternalSpeed);

	OwnerPawn->SetActorRotation(TargetRotation);
}
