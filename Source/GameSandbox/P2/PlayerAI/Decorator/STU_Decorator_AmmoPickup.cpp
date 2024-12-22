// Copyright (C) 2023, IKinder

#include "STU_Decorator_AmmoPickup.h"
#include "AIController.h"
#include "P2/Component/STU_WeaponComponent.h"

USTU_Decorator_AmmoPickup::USTU_Decorator_AmmoPickup()
{
	NodeName = "Ammo Pickup";
}

bool USTU_Decorator_AmmoPickup::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return false;

	USTU_WeaponComponent* WeaponComponent = Controller->GetPawn()->FindComponentByClass<USTU_WeaponComponent>();
	if (!WeaponComponent) return false;

	return WeaponComponent->NeedAmmo(WeaponType);
}
