// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STU_Decorator_HealthPercent.generated.h"

UCLASS()
class PROJECT02_API USTU_Decorator_HealthPercent : public UBTDecorator
{
	GENERATED_BODY()

public:
	USTU_Decorator_HealthPercent();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI")
	float HealthPercent = 0.5f;
};
