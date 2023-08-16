// Copyright (C) 2023, IKinder

#include "P10/Public/Player/P10Character.h"

AP10Character::AP10Character()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP10Character::BeginPlay()
{
	Super::BeginPlay();
}

void AP10Character::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP10Character::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

