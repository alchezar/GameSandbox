// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "TG_Task_FocusAtActor.generated.h"

UCLASS()
class PROJECT04_API UTG_Task_FocusAtActor : public UBTTaskNode
{
	GENERATED_BODY()

public:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder")
	FBlackboardKeySelector FocusKey;
};
