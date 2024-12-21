// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ARBTService_CheckAttackRange.generated.h"


UCLASS()
class PROJECT06_API UARBTService_CheckAttackRange : public UBTService
{
	GENERATED_BODY()

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	void CheckDistance(UBehaviorTreeComponent& OwnerComp);

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, Category = "C++")
	FBlackboardKeySelector AttackRangeKey;
	UPROPERTY(EditAnywhere, Category = "C++")
	float AcceptableDistance = 500.f;
};
