// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STU_Service_Fire.generated.h"

UCLASS()
class GAMESANDBOX_API USTU_Service_Fire : public UBTService
{
	GENERATED_BODY()

public:
	USTU_Service_Fire();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI")
	FBlackboardKeySelector EnemyActorKey;
};
