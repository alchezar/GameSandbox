// Copyright (C) 2023, IKinder

#include "P7/Public/Item/P7Item.h"

AP7Item::AP7Item()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP7Item::BeginPlay()
{
	Super::BeginPlay();
}

void AP7Item::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

