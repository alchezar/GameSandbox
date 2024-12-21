// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TG_Service_SetFocus.generated.h"

UCLASS()
class PROJECT04_API UTG_Service_SetFocus : public UBTService
{
	GENERATED_BODY()

public:
	UTG_Service_SetFocus();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder")
	FBlackboardKeySelector FocusActorKey;
};
