// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Equipment/Weapon/P12MeleeWeaponItem.h"

#include "P12/Public/Player/P12BaseCharacter.h"
#include "P12/Public/Player/AnimNotify/P12AnimNotifyWindow_MeleeAttack.h"

AP12MeleeWeaponItem::AP12MeleeWeaponItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneRootComponent");

	WeaponMesh = CreateDefaultSubobject<UStaticMeshComponent>("WeaponSkeletalMesh");
	WeaponMesh->SetupAttachment(RootComponent);

	EquippedSocketName = SocketCharacterWeapon;
}

void AP12MeleeWeaponItem::BeginPlay()
{
	Super::BeginPlay();

	WeaponMesh->SetVisibility(false);
	OnEquipmentStateChanged.AddUObject(this, &ThisClass::OnEquipmentStateChangedHandle);
}

void AP12MeleeWeaponItem::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP12MeleeWeaponItem::InitAnimNotify()
{
	Super::InitAnimNotify();

	for (auto [Key, Value] : Attacks)
	{
		TArray<FAnimNotifyEvent> NotifyEvents = Value.Montage->Notifies;
		for (auto NotifyEvent : NotifyEvents)
		{
			auto* MeleeAttackNotify = Cast<UP12AnimNotifyWindow_MeleeAttack>(NotifyEvent.NotifyStateClass);
			if (!MeleeAttackNotify)
			{
				continue;
			}
			MeleeAttackNotify->OnMeleeAttackStart.AddUObject(this, &ThisClass::OnMeleeAttackStartHandle);
			MeleeAttackNotify->OnMeleeAttackEnd.AddUObject(this, &ThisClass::OnMeleeAttackEndHandle);
		}
	}
}

void AP12MeleeWeaponItem::StartAttack(EP12MeleeAttackType AttackType)
{
	AP12BaseCharacter* CharacterOwner = GetCachedCharacter().Get();
	CurrentAttack = Attacks.Find(AttackType);
	if (!CharacterOwner || !CurrentAttack || !CurrentAttack->Montage)
	{
		return;
	}
	CharacterOwner->PlayAnimMontage(CurrentAttack->Montage);
}

void AP12MeleeWeaponItem::OnMeleeAttackStartHandle(USkeletalMeshComponent* SkeletalMeshComponent) 
{
	const AP12BaseCharacter* CharacterOwner = GetCachedCharacter().Get();
	if (CharacterOwner && CharacterOwner->GetMesh() == SkeletalMeshComponent)
	{
		return;
	}
	// TODO: start attack
}

void AP12MeleeWeaponItem::OnMeleeAttackEndHandle(USkeletalMeshComponent* SkeletalMeshComponent) 
{
	const AP12BaseCharacter* CharacterOwner = GetCachedCharacter().Get();
	if (CharacterOwner && CharacterOwner->GetMesh() == SkeletalMeshComponent)
	{
		return;
	}

	// TODO: end attack
	
	CurrentAttack = nullptr;
}

void AP12MeleeWeaponItem::OnEquipmentStateChangedHandle(const bool bEquipped)
{
	WeaponMesh->SetVisibility(bEquipped);
}
