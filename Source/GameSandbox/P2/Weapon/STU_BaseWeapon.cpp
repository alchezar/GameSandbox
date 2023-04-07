// Copyright (C) 2023, IKinder

#include "STU_BaseWeapon.h"
#include "Components/SkeletalMeshComponent.h"

ASTU_BaseWeapon::ASTU_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = false;

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponMeshComponent");
	SetRootComponent(WeaponMesh);
}

void ASTU_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}
