// Copyright © 2025, Ivan Kinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "P17BTTRotateToFaceTarget.generated.h"

struct FP17RotateToFaceTarget
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
};

UCLASS()
class PROJECT17_API UP17BTTRotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()

	/// ------------------------------------------------------------------------
	/// @name Unreal
	/// ------------------------------------------------------------------------
private:
	UP17BTTRotateToFaceTarget();
	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;
	virtual uint16 GetInstanceMemorySize() const override;
	virtual FString GetStaticDescription() const override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/// ------------------------------------------------------------------------
	/// @name This
	/// ------------------------------------------------------------------------
private:
	bool GetHasReachedAnglePrecision(const APawn* QueryPawn, const AActor* TargetActor) const;

	/// ------------------------------------------------------------------------
	/// @name Fields
	/// ------------------------------------------------------------------------
private:
	UPROPERTY(EditAnywhere)
	float AnglePrecision = 10.f;
	UPROPERTY(EditAnywhere)
	float RotationInterpSpeed = 5.f;
	UPROPERTY(EditAnywhere)
	FBlackboardKeySelector InTargetToFaceKey;
};
