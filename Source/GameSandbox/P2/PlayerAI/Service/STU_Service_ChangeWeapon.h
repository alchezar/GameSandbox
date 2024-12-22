// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "STU_Service_ChangeWeapon.generated.h"

class USTU_AIWeaponComponent;

UCLASS()
class GAMESANDBOX_API USTU_Service_ChangeWeapon : public UBTService
{
	GENERATED_BODY()

public:
	USTU_Service_ChangeWeapon();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI", meta = (ClampMin = "0.0", ClampMax = "1.0"))
	float Probability = 0.5f;
};
