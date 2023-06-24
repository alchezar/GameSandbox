// Copyright (C) 2023, IKinder

#include "P7/Public/Item/P7Item.h"

AP7Item::AP7Item()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>("ItemMeshComponent");
	SetRootComponent(ItemMesh);
}

void AP7Item::BeginPlay()
{
	Super::BeginPlay();
	
}

void AP7Item::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}

