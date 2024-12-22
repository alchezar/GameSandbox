// Copyright (C) 2023, IKinder

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "STU_Decorator_AmmoPickup.generated.h"

class ASTU_BaseWeapon;

UCLASS()
class GAMESANDBOX_API USTU_Decorator_AmmoPickup : public UBTDecorator
{
	GENERATED_BODY()

public:
	USTU_Decorator_AmmoPickup();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Kinder | AI")
	TSubclassOf<ASTU_BaseWeapon> WeaponType;
};
