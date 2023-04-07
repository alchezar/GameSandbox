// Copyright (C) 2023, IKinder

#include "STU_BaseWeapon.h"

ASTU_BaseWeapon::ASTU_BaseWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_BaseWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_BaseWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
