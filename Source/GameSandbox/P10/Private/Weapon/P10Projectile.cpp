// Copyright (C) 2023, IKinder

#include "P10/Public/Weapon/P10Projectile.h"

AP10Projectile::AP10Projectile()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP10Projectile::BeginPlay()
{
	Super::BeginPlay();
}

void AP10Projectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

