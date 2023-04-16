// Copyright (C) 2023, IKinder

#include "STU_AIWeaponComponent.h"
#include "Weapon/STU_BaseWeapon.h"

USTU_AIWeaponComponent::USTU_AIWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void USTU_AIWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTU_AIWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void USTU_AIWeaponComponent::StartFire()
{
	if (!CanFire()) return;

	if (CurrentWeapon->IsAmmoEmpty())
	{
		NextWeapon();
	}
	else
	{
		Super::StartFire();
	}
}

void USTU_AIWeaponComponent::NextWeapon()
{
	if (!CanEquip()) return;

	int32 NextIndex = (CurrentWeaponIndex + 1) % Weapons.Num();
	while (NextIndex != CurrentWeaponIndex)
	{
		if (!Weapons[NextIndex]->IsAmmoEmpty()) break;
		NextIndex = (NextIndex + 1) % Weapons.Num();
	}

	if (NextIndex != CurrentWeaponIndex)
	{
		CurrentWeaponIndex = NextIndex;

		PlayAnimMontage(EquipAnimation);
		bWeaponChanging  = true;
		bWeaponReloading = false;
		// EquipWeapon(CurrentWeaponIndex);
	}
}
