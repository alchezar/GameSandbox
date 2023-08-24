// Copyright (C) 2023, IKinder

#include "P10/Public/Weapon/P10Weapon.h"

AP10Weapon::AP10Weapon()
{
	PrimaryActorTick.bCanEverTick = true;

	RootOffsetComponent = CreateDefaultSubobject<USceneComponent>("RootOffsetSceneComponent");
	SetRootComponent(RootOffsetComponent);
	
	WeaponMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("WeaponSkeletalMeshComponent");
	WeaponMeshComponent->SetupAttachment(RootComponent);
	WeaponMeshComponent->SetRelativeLocationAndRotation(LocationOffset, RotationOffset);
}

void AP10Weapon::BeginPlay()
{
	Super::BeginPlay();
}

void AP10Weapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
