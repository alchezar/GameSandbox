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
