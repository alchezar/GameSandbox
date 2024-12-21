// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "TG_Service_UpdateLastLocation.generated.h"

class ATG_BaseCharacter;

UCLASS()
class PROJECT04_API UTG_Service_UpdateLastLocation : public UBTService
{
	GENERATED_BODY()

public:
	UTG_Service_UpdateLastLocation();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder")
	FBlackboardKeySelector TargetKey;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder")
	FBlackboardKeySelector LastSeenLocationKey;
};
