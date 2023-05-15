// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TG_Task_ClearBlackboardValue.generated.h"

UCLASS()
class GAMESANDBOX_API UTG_Task_ClearBlackboardValue : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kider")
	FBlackboardKeySelector KeyToClear;
};
