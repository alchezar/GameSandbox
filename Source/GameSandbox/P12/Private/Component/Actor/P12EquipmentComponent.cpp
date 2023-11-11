// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Actor/P12EquipmentComponent.h"

#include "Net/UnrealNetwork.h"
#include "P12/Public/Actor/Equipment/Throwable/P12ThrowableItem.h"
#include "P12/Public/Actor/Equipment/Weapon/P12MeleeWeaponItem.h"
#include "P12/Public/Actor/Equipment/Weapon/P12RangeWeaponItem.h"
#include "P12/Public/Player/P12BaseCharacter.h"

UP12EquipmentComponent::UP12EquipmentComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}

void UP12EquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UP12EquipmentComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ThisClass, CurrentEquippedSlot);
	DOREPLIFETIME(ThisClass, AmmunitionArray);
	DOREPLIFETIME(ThisClass, ItemsArray);
}

void UP12EquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	check(GetOwner()->IsA<AP12BaseCharacter>())
	CachedCharacter = StaticCast<AP12BaseCharacter*>(GetOwner());

	CreateLoadout();
	AutoEquip();
	CachedCharacter->OnLoadoutCreated.Broadcast(this);
}

void UP12EquipmentComponent::CreateLoadout()
{
	if (GetOwner()->GetLocalRole() < ROLE_Authority)
	{
		return;
	}
	
	AmmunitionArray.AddZeroed(static_cast<uint32>(EP12AmmunitionType::MAX));
	for (const TPair<EP12AmmunitionType, int32>& AmmoPair : MaxAmmunitionAmount)
	{
		AmmunitionArray[static_cast<uint32>(AmmoPair.Key)] = FMath::Max(AmmoPair.Value, 0);
	}

	ItemsArray.AddZeroed(static_cast<uint32>(EP12EquipmentSlot::MAX));
	for (const TPair<EP12EquipmentSlot, TSubclassOf<AP12EquipableItem>>& ItemPair : ItemsLoadout)
	{
		if (!ItemPair.Value)
		{
			continue;
		}
		AP12EquipableItem* Item = GetWorld()->SpawnActorDeferred<AP12EquipableItem>(ItemPair.Value, FTransform::Identity, CachedCharacter.Get());
		Item->AttachToComponent(CachedCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, Item->GetUnEquippedSocketName());
		Item->CacheEquipmentComponent(this);
		Item->Unequip();
		Item->FinishSpawning(FTransform::Identity);
		ItemsArray[static_cast<uint32>(ItemPair.Key)] = Item;
	}
}

EP12EquipableItemType UP12EquipmentComponent::GetCurrentEquippedItemType() const
{
	if (!CurrentEquippedItem)
	{
		return EP12EquipableItemType::None;
	}
	return CurrentEquippedItem->GetItemType();
}

int32 UP12EquipmentComponent::GetMaxAvailableAmmoAmount(const EP12AmmunitionType AmmoType)
{
	return AmmunitionArray[static_cast<uint32>(AmmoType)];
}

void UP12EquipmentComponent::DecreaseMaxAvailableAmmoAmount(const EP12AmmunitionType AmmoType, const int32 AmmoToDecrease)
{
	AmmunitionArray[static_cast<uint32>(AmmoType)] -= AmmoToDecrease ;
}

void UP12EquipmentComponent::EquipItemInSlot(EP12EquipmentSlot Slot)
{	
	if (!CachedCharacter.IsValid() || CachedCharacter->GetIsEquipping())
	{
		return;
	}

	UnEquipCurrentItem();

	if (ItemsArray.IsEmpty())
	{
		return;
	}
	
	/* Equip new weapon */
	CurrentEquippedItem = ItemsArray[static_cast<uint32>(Slot)];
	CurrentEquippedWeapon = Cast<AP12RangeWeaponItem>(CurrentEquippedItem);
	CurrentThrowableItem  = Cast<AP12ThrowableItem>(CurrentEquippedItem);
	CurrentMeleeWeapon    = Cast<AP12MeleeWeaponItem>(CurrentEquippedItem);
	
	if (!CurrentEquippedItem)
	{
		return;
	}
	CurrentEquippedSlot = Slot;
	
	if (UAnimMontage* EquipMontage = CurrentEquippedItem->GetCharacterEquipAnimMontage())
	{
		CachedCharacter->PlayEquippingItem(EquipMontage);
		/* ::EquipCurrentItem()                  On notify window begin.
		 * ::CachedCharacter->EndEquippingItem() On notify window end.   */
	}
	else
	{
		EquipCurrentItem();
	}
	CurrentEquippedItem->Equip();

	if (GetOwner()->GetLocalRole() == ROLE_AutonomousProxy)
	{
		Server_EquipItemInSlot(CurrentEquippedSlot);
	}
}

void UP12EquipmentComponent::EquipNextItem() 
{
	const uint32 CurrentSlotIndex = static_cast<uint32>(CurrentEquippedSlot);
	for (int i = 1; i <= ItemsArray.Num(); ++i)
	{
		const uint32 NextIndex = (CurrentSlotIndex + i) % ItemsArray.Num();
		if (!ItemsArray[NextIndex] || IgnoredSlotsWhileSwitching.Contains(static_cast<EP12EquipmentSlot>(NextIndex)))
		{
			continue;
		}
		EquipItemInSlot(static_cast<EP12EquipmentSlot>(NextIndex));
		break;
	}
}

void UP12EquipmentComponent::EquipPreviousItem() 
{
	const uint32 CurrentSlotIndex = static_cast<uint32>(CurrentEquippedSlot);
	for (int i = 1; i < ItemsArray.Num(); ++i)
	{
		const int32 StepBack = CurrentSlotIndex - i;
		const uint32 PrevIndex = StepBack > 0 ? StepBack : StepBack + ItemsArray.Num() - 1;
		if (!ItemsArray[PrevIndex] || IgnoredSlotsWhileSwitching.Contains(static_cast<EP12EquipmentSlot>(PrevIndex)))
		{
			continue;
		}
		EquipItemInSlot(static_cast<EP12EquipmentSlot>(PrevIndex));
		break;
	}
}

void UP12EquipmentComponent::UnEquipCurrentItem()
{
	if (!CurrentEquippedItem)
	{
		return;
	}

	if (!IgnoredSlotsWhileSwitching.Contains(CurrentEquippedSlot))
	{
		PreviousEquippedSlot = CurrentEquippedSlot;
	}
	
	CurrentEquippedItem->AttachItem(CurrentEquippedItem->GetUnEquippedSocketName());
	CurrentEquippedItem->Unequip();
	
	CurrentEquippedSlot = EP12EquipmentSlot::None;
	if (!CurrentEquippedWeapon)
	{
		return;
	}
	CurrentEquippedWeapon->FireInput(false);
	CurrentEquippedWeapon->OnFullReloadedHandle(CachedCharacter->GetMesh());
}

void UP12EquipmentComponent::EquipCurrentItem()
{
	if (CurrentEquippedItem)
	{
		CurrentEquippedItem->AttachItem(CurrentEquippedItem->GetEquippedSocketName());
		CurrentEquippedItem->ToggleReticle(!IgnoredSlotsWhileSwitching.Contains(CurrentEquippedSlot));
	}
	if (CurrentMeleeWeapon)
	{
		CurrentMeleeWeapon->ToggleReticle(false);
	}
}

void UP12EquipmentComponent::TakeCurrentThrowableItem()
{
	check(CurrentThrowableItem)
	
	CachedCharacter->SetIsEquipping(true);

	CurrentThrowableItem->TakeThrowable();
}

void UP12EquipmentComponent::LaunchCurrentThrowableItem()
{
	check(CurrentThrowableItem)

	CurrentThrowableItem->Throw();
	
	CachedCharacter->SetIsEquipping(false);
	EquipItemInSlot(PreviousEquippedSlot);
}

void UP12EquipmentComponent::AutoEquip()
{
	if (AutoEquipItemInSlot == EP12EquipmentSlot::None || !ItemsLoadout.Contains(AutoEquipItemInSlot))
	{
		return;
	}
	EquipItemInSlot(AutoEquipItemInSlot);
}

void UP12EquipmentComponent::Server_EquipItemInSlot_Implementation(EP12EquipmentSlot Slot)
{
	EquipItemInSlot(Slot);
}

void UP12EquipmentComponent::OnRep_CurrentEquippedSlot(EP12EquipmentSlot OldCurrentSlot)
{
	EquipItemInSlot(CurrentEquippedSlot);
}

void UP12EquipmentComponent::OnRep_ItemsArray()
{
	for (AP12EquipableItem* Item : ItemsArray)
	{
		if (!Item)
		{
			continue;
		}
		Item->Unequip();
	}
}
