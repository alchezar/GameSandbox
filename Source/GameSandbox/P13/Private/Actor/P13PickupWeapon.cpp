// Copyright © 2024, IKinder

#include "P13/Public/Actor/P13PickupWeapon.h"

#include "P13/Public/Component/Actor/P13InventoryComponent.h"

AP13PickupWeapon::AP13PickupWeapon()
{
	
}

void AP13PickupWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AP13PickupWeapon::InitDrop(const FP13WeaponDrop* DropWeaponInfo)
{
	/* Save init info. */
	bDropped = true;
	Mesh->SetStaticMesh(DropWeaponInfo->WeaponMesh);
	WeaponSlot = DropWeaponInfo->WeaponInfo;

	AmmoSlot = {WeaponSlot.AmmoType, DropWeaponInfo->WeaponInfo.DynamicInfo.Round, DropWeaponInfo->MaxCount};

	/* Start timer. */
	GetWorld()->GetTimerManager().SetTimer(DropTimer, ActivationDelay, false);
}

void AP13PickupWeapon::OnCollisionBeginOverlapHandle(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (GetWorld()->GetTimerManager().IsTimerActive(DropTimer))
	{
		return;
	}
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
	
	/* If weapon wasn't ever picked up before - magazine will be full. */
	if (!bDropped)
	{
		WeaponSlot.DynamicInfo.Round = WeaponSlot.MaxRound;
	}
	/* Try to add weapon to the inventory. */
	const bool bWeaponTaken = PickupInterface->TryTakeWeaponToInventory(WeaponSlot);
		
	/* When we successfully took the weapon, but there is no slot for its ammo in the inventory -
	 * try to add an empty one, by resetting count in ammo slot. */
	AmmoSlot.Count = bWeaponTaken ? 0 : AmmoSlot.Count;
		
	/* Or if we already have this weapon in the inventory - simply add one magazine to its ammo slot. */
	if (!PickupInterface->TryTakeAmmoToInventory(AmmoSlot) && !bWeaponTaken)
	{
		return;
	}
	OnPickupSuccess();
}
