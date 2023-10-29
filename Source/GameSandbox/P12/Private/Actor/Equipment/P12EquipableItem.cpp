// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Equipment/P12EquipableItem.h"

#include "P12/Public/Component/Actor/P12EquipmentComponent.h"
#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Player/AnimNotify/P12AnimNotifyWindow_Equipping.h"

void AP12EquipableItem::CacheEquipmentComponent(const UP12EquipmentComponent* EquipmentComp)
{
	CachedEquipment = EquipmentComp;
}

void AP12EquipableItem::BeginPlay()
{
	Super::BeginPlay();
	InitAnimNotify();
	
	check(GetOwner()->IsA<AP12BaseCharacter>())
	CachedCharacter = StaticCast<AP12BaseCharacter*>(GetOwner());
}

void AP12EquipableItem::InitAnimNotify()
{
	if (!CharacterEquipAnimMontage)
	{
		return;
	}
	TArray<FAnimNotifyEvent> NotifyEvents = CharacterEquipAnimMontage->Notifies;
	for (auto NotifyEvent : NotifyEvents)
	{
		if (UP12AnimNotifyWindow_Equipping* NotifyWindow = Cast<UP12AnimNotifyWindow_Equipping>(NotifyEvent.NotifyStateClass))
		{
			NotifyWindow->OnEquippingStarted.AddUObject(this, &ThisClass::OnEquippingStartedHandle);
			NotifyWindow->OnEquippingFinished.AddUObject(this, &ThisClass::OnEquippingFinishedHandle);
		}
	}
}

void AP12EquipableItem::OnEquippingStartedHandle(USkeletalMeshComponent* SkeletalMeshComponent) 
{
	CachedEquipment->EquipCurrentItem();
}

void AP12EquipableItem::OnEquippingFinishedHandle(USkeletalMeshComponent* SkeletalMeshComponent) 
{
	CachedCharacter->EndEquippingItem();
}

void AP12EquipableItem::AttachItem(const FName AttachSocketName)
{
	AttachToComponent(CachedCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, AttachSocketName);
}

void AP12EquipableItem::Equip() 
{
	OnEquipmentStateChanged.Broadcast(true);
}

void AP12EquipableItem::Unequip() 
{
	OnEquipmentStateChanged.Broadcast(false);

}

