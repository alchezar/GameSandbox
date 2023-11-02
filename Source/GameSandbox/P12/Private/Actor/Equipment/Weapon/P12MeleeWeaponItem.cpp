// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Equipment/Weapon/P12MeleeWeaponItem.h"

#include "Engine/DamageEvents.h"
#include "P12/Public/Component/Scene/P12MeleeHitRegistration.h"
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

	GetComponents<UP12MeleeHitRegistration>(HitRegistrators);
	for (UP12MeleeHitRegistration* HitRegistrator : HitRegistrators)
	{
		HitRegistrator->OnMeleeRegistered.AddUObject(this, &ThisClass::ProcessHit);
	}
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
	if (CharacterOwner && CharacterOwner->GetMesh() != SkeletalMeshComponent)
	{
		return;
	}
	/* Start attack window */
	ToggleHitRegistration(true);
}

void AP12MeleeWeaponItem::OnMeleeAttackEndHandle(USkeletalMeshComponent* SkeletalMeshComponent) 
{
	const AP12BaseCharacter* CharacterOwner = GetCachedCharacter().Get();
	if (CharacterOwner && CharacterOwner->GetMesh() != SkeletalMeshComponent)
	{
		return;
	}

	/* End attack window */
	ToggleHitRegistration(false);
	
	CurrentAttack = nullptr;
}

void AP12MeleeWeaponItem::OnEquipmentStateChangedHandle(const bool bEquipped)
{
	WeaponMesh->SetVisibility(bEquipped);
}

void AP12MeleeWeaponItem::ProcessHit(const FHitResult& HitResult, const FVector& HitDirection)
{
	AActor* ActorToHit = HitResult.GetActor();
	if (!ActorToHit || ActorsToHit.Contains(ActorToHit) || !CurrentAttack)
	{
		return;
	}
	FPointDamageEvent DamageEvent;
	DamageEvent.HitInfo = HitResult;
	DamageEvent.ShotDirection = HitDirection;
	DamageEvent.DamageTypeClass = CurrentAttack->DamageType;
	AController* HitInstigator = GetCachedCharacter().Get() ? GetCachedCharacter()->Controller : nullptr;
	ActorToHit->TakeDamage(CurrentAttack->Damage, DamageEvent, HitInstigator, GetOwner());
	ActorsToHit.Add(ActorToHit);
}

void AP12MeleeWeaponItem::ToggleHitRegistration(const bool bEnable)
{
	ActorsToHit.Empty();
	for (UP12MeleeHitRegistration* HitRegistrator : HitRegistrators)
	{
		HitRegistrator->SetIsHitRegistration(bEnable);
	}
}
