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
	CurrentEquippedItem = GetWorld()->SpawnActor<AP12RangeWeaponItem>(SideArmClass);
	CurrentEquippedItem->AttachToComponent(CachedCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, SideArmSocketName);
}

EP12EquipablItemType UP12EquipmentComponent::GetCurrentEquippedItemType() const
{
	if (!CurrentEquippedItem)
	{
		return EP12EquipablItemType::None;
	}
	return CurrentEquippedItem->GetItemType();
}