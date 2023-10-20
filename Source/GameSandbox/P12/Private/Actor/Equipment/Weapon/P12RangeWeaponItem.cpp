// Copyright (C) 2023, IKinder

#include "P12/Public/Actor/Equipment/Weapon/P12RangeWeaponItem.h"

#include "P12/Public/Component/Scene/P12WeaponBarrelComponent.h"

AP12RangeWeaponItem::AP12RangeWeaponItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>("SceneWeaponRootComponent");

	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	WeaponMesh->SetupAttachment(RootComponent);
	WeaponMesh->SetRelativeLocation(WeaponMeshOffset);

	WeaponBarrel = CreateDefaultSubobject<UP12WeaponBarrelComponent>("WeaponBarrelSceneComponent");
	WeaponBarrel->SetupAttachment(WeaponMesh, MuzzleSocketName);
}

void AP12RangeWeaponItem::BeginPlay()
{
	Super::BeginPlay();
}

void AP12RangeWeaponItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

