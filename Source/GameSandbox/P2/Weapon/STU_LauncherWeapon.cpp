// Copyright (C) 2023, IKinder

#include "STU_LauncherWeapon.h"

ASTU_LauncherWeapon::ASTU_LauncherWeapon()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_LauncherWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_LauncherWeapon::StartFire()
{
	MakeShot();
}

void ASTU_LauncherWeapon::StopFire()
{}

void ASTU_LauncherWeapon::MakeShot()
{}
