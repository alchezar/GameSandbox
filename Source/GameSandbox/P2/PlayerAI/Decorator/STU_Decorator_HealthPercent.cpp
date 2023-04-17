// Copyright (C) 2023, IKinder

#include "STU_Decorator_HealthPercent.h"
#include "AIController.h"
#include "Component/STU_HealthComponent.h"

USTU_Decorator_HealthPercent::USTU_Decorator_HealthPercent()
{
	NodeName = "Health Percent";
}

bool USTU_Decorator_HealthPercent::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	const USTU_HealthComponent* HealthComponent = Controller->GetPawn()->FindComponentByClass<USTU_HealthComponent>();
	if (!HealthComponent || HealthComponent->IsDead()) return false;

	return HealthComponent->GetHealthPercent() < HealthPercent;
}
