// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ARBTTask_RangedAttack.generated.h"

UCLASS()
class GAMESANDBOX_API UARBTTask_RangedAttack : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UARBTTask_RangedAttack();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, Category = "C++")
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, Category = "C++")
	FName HandSocketName = "GripPoint";
	UPROPERTY(EditAnywhere, Category = "C++")
	TSubclassOf<AActor> ProjectileClass;
	UPROPERTY(EditAnywhere, Category = "C++")
	float MaxSpread = 3;
};
