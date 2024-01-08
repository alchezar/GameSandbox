// Copyright © 2024, IKinder

#include "P13/Public/Actor/P13PickupAmmo.h"

#include "Components/SphereComponent.h"
#include "P13/Public/Component/Actor/P13InventoryComponent.h"

AP13PickupAmmo::AP13PickupAmmo()
{
	
}

void AP13PickupAmmo::BeginPlay()
{
	Super::BeginPlay();

	Collision->AttachToComponent(Mesh, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AP13PickupAmmo::OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UP13InventoryComponent* InventoryComponent = OtherActor->FindComponentByClass<UP13InventoryComponent>();
	if (!InventoryComponent)
	{
		return;
	}
	IP13PickupInterface* PickupInterface = Cast<IP13PickupInterface>(InventoryComponent);
	if (!PickupInterface)
	{
		return;
	}
		
	/* Try to add ammo to the inventory. */
	if (!PickupInterface->TryTakeAmmoToInventory(AmmoSlot))
	{
		return;
	}
	OnPickupSuccess();
}
