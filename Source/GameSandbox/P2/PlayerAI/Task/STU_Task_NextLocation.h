// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "STU_Task_NextLocation.generated.h"

UCLASS()
class GAMESANDBOX_API USTU_Task_NextLocation : public UBTTaskNode
{
	GENERATED_BODY()

public:
	USTU_Task_NextLocation();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI")
	float Radius = 1000.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI")
	FBlackboardKeySelector AimLocationKey;	
};
