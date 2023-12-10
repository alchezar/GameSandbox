// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "P12Service_P12Fire.generated.h"

UCLASS()
class GAMESANDBOX_API UP12Service_P12Fire : public UBTService
{
	GENERATED_BODY()

public:
	UP12Service_P12Fire();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	FBlackboardKeySelector TargetActorKey;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "C++")
	float MaxFireDistance = 500.f;
};
