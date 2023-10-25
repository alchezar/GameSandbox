// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Actor/P12EquipmentComponent.h"

#include "P12/Public/Actor/Equipment/Weapon/P12RangeWeaponItem.h"
#include "P12/Public/Player/P12BaseCharacter.h"

UP12EquipmentComponent::UP12EquipmentComponent()
{
	
}

void UP12EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner()->IsA<AP12BaseCharacter>())
	CachedCharacter = StaticCast<AP12BaseCharacter*>(GetOwner());

	CreateLoadout();
}

void UP12EquipmentComponent::CreateLoadout()
{
	AmmunitionArray.AddZeroed(static_cast<uint32>(EP12AmmunitionType::MAX));
	for (const TPair<EP12AmmunitionType, int32>& AmmoPair : MaxAmmunitionAmount)
	{
		AmmunitionArray[static_cast<uint32>(AmmoPair.Key)] = FMath::Max(AmmoPair.Value, 0);
	}
	
	if (!SideArmClass)
	{
		return;
	}
	// CurrentEquippedWeapon = GetWorld()->SpawnActor<AP12RangeWeaponItem>(SideArmClass);
	// CurrentEquippedWeapon->SetOwner(CachedCharacter.Get());
	CurrentEquippedWeapon = GetWorld()->SpawnActorDeferred<AP12RangeWeaponItem>(SideArmClass, FTransform::Identity, CachedCharacter.Get());
	CurrentEquippedWeapon->AttachToComponent(CachedCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SideArmSocketName);
	CurrentEquippedWeapon->FinishSpawning(FTransform::Identity);
}

EP12EquipableItemType UP12EquipmentComponent::GetCurrentEquippedItemType() const
{
	if (!CurrentEquippedWeapon)
	{
		return EP12EquipableItemType::None;
	}
	return CurrentEquippedWeapon->GetItemType();
}

int32 UP12EquipmentComponent::GetMaxAvailableAmmoAmount(const EP12AmmunitionType AmmoType)
{
	return AmmunitionArray[static_cast<uint32>(AmmoType)];
}

void UP12EquipmentComponent::DecreaseMaxAvailableAmmoAmount(const EP12AmmunitionType AmmoType, const int32 AmmoToDecrease)
{
	AmmunitionArray[static_cast<uint32>(AmmoType)] -= AmmoToDecrease ;
}
