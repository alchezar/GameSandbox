// Copyright (C) 2023, IKinder

#include "STU_Service_ChangeWeapon.h"

#include "AIController.h"
#include "Component/STU_AIWeaponComponent.h"
#include "PlayerAI/STU_AICharacter.h"

USTU_Service_ChangeWeapon::USTU_Service_ChangeWeapon()
{
	NodeName = "Change Weapon";
}

void USTU_Service_ChangeWeapon::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	const AAIController* Controller = OwnerComp.GetAIOwner();
	if (!Controller) return;

	const ASTU_AICharacter* Character = Cast<ASTU_AICharacter>(Controller->GetPawn());
	if (!Character) return;

	USTU_AIWeaponComponent* WeaponComponent = Character->FindComponentByClass<USTU_AIWeaponComponent>();
	if (WeaponComponent && Probability > 0 && FMath::FRand() <= Probability)
	{
		WeaponComponent->NextWeapon();
	}

	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
}