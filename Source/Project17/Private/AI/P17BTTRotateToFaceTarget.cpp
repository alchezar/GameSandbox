// Copyright © 2025, Ivan Kinder

#include "AI/P17BTTRotateToFaceTarget.h"

#include "AIController.h"
#include "Project17.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/KismetMathLibrary.h"

UP17BTTRotateToFaceTarget::UP17BTTRotateToFaceTarget()
{
	NodeName = "Native rotate to face target";
	bNotifyTick = true;
	bNotifyTaskFinished = true;
	bCreateNodeInstance = false;
	INIT_TASK_NODE_NOTIFY_FLAGS();
	InTargetToFaceKey.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, InTargetToFaceKey), AActor::StaticClass());
}

void UP17BTTRotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	const UBlackboardData* BlackboardAsset = GetBlackboardAsset();
	RETURN_IF(!BlackboardAsset,)
	InTargetToFaceKey.ResolveSelectedKey(*BlackboardAsset);
}

uint16 UP17BTTRotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FP17RotateToFaceTarget);
}

FString UP17BTTRotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = InTargetToFaceKey.SelectedKeyName.ToString();
	return FString::Printf(L"Smoothly rotates to face %s\nKey until the angle precision %s is reached.", *KeyDescription, *FString::SanitizeFloat(AnglePrecision));
}

EBTNodeResult::Type UP17BTTRotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	RETURN_IF(!Blackboard, EBTNodeResult::Failed);
	AActor* TargetActor = Cast<AActor>(Blackboard->GetValueAsObject(InTargetToFaceKey.SelectedKeyName));

	const AAIController* OwnerController = OwnerComp.GetAIOwner();
	RETURN_IF(!OwnerController, EBTNodeResult::Failed);
	APawn* OwnerPawn = OwnerController->GetPawn();

	FP17RotateToFaceTarget* Memory = CastInstanceNodeMemory<FP17RotateToFaceTarget>(NodeMemory);
	WARN_RETURN_IF(!Memory, EBTNodeResult::Failed);
	Memory->OwningPawn = OwnerPawn;
	Memory->TargetActor = TargetActor;
	RETURN_IF(!Memory->IsValid(), EBTNodeResult::Failed);

	RETURN_IF(!GetHasReachedAnglePrecision(OwnerPawn, TargetActor), EBTNodeResult::InProgress);
	Memory->Reset();
	return EBTNodeResult::Succeeded;
}

void UP17BTTRotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	FP17RotateToFaceTarget* Memory = CastInstanceNodeMemory<FP17RotateToFaceTarget>(NodeMemory);
	if (!Memory || !Memory->IsValid())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
	APawn* OwningPawn = Memory->OwningPawn.Get();
	const AActor* TargetActor = Memory->TargetActor.Get();

	if (GetHasReachedAnglePrecision(OwningPawn, TargetActor))
	{
		Memory->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}

	const FRotator LookAtRotation = (TargetActor->GetActorLocation() - OwningPawn->GetActorLocation()).Rotation();
	const FRotator TargetRotation = FMath::RInterpTo(OwningPawn->GetActorRotation(), LookAtRotation, DeltaSeconds, RotationInterpSpeed);
	OwningPawn->SetActorRotation(TargetRotation);
}

bool UP17BTTRotateToFaceTarget::GetHasReachedAnglePrecision(const APawn* QueryPawn, const AActor* TargetActor) const
{
	const FVector OwnerForward = QueryPawn->GetActorForwardVector();
	const FVector OwnerToTarget = (TargetActor->GetActorLocation() - QueryPawn->GetActorLocation()).GetSafeNormal();

	const float DotProduct = OwnerForward | OwnerToTarget;
	const float AngleDiff = UKismetMathLibrary::DegAcos(DotProduct);

	return AngleDiff <= AnglePrecision;
}
