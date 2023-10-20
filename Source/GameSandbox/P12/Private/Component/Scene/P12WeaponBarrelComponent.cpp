// Copyright (C) 2023, IKinder

#include "P12/Public/Component/Scene/P12WeaponBarrelComponent.h"

UP12WeaponBarrelComponent::UP12WeaponBarrelComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}

void UP12WeaponBarrelComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UP12WeaponBarrelComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

