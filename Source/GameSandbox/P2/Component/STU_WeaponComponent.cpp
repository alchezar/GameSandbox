// Copyright (C) 2023, IKinder

#include "STU_WeaponComponent.h"
#include "GameFramework/Character.h"
#include "GameSandbox/P2/Weapon/STU_BaseWeapon.h"

USTU_WeaponComponent::USTU_WeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTU_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	SpawnWeapon();
}

void USTU_WeaponComponent::SpawnWeapon()
{
	if (!GetWorld()) return;

	auto* Character = Cast<ACharacter>(GetOwner());
	if (!Character) return;

	CurrentWeapon = GetWorld()->SpawnActor<ASTU_BaseWeapon>(WeaponClass);
	if (!CurrentWeapon) return;

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, false);
	CurrentWeapon->AttachToComponent(Character->GetMesh(), AttachmentRules, WeaponAttachPointName);
	CurrentWeapon->SetOwner(Character);
}

void USTU_WeaponComponent::Fire()
{
	if (!CurrentWeapon) return;
	CurrentWeapon->Fire();
}
