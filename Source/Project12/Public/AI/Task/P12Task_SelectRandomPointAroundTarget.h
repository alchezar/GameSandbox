// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "P12Task_SelectRandomPointAroundTarget.generated.h"

UCLASS()
class PROJECT12_API UP12Task_SelectRandomPointAroundTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UP12Task_SelectRandomPointAroundTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | AI")
	float Radius = 500.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | AI")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++ | AI")
	FBlackboardKeySelector LocationKey;
};
