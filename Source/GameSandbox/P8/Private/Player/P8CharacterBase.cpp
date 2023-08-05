// Copyright (C) 2023, IKinder

#include "P8/Public/Player/P8CharacterBase.h"

AP8CharacterBase::AP8CharacterBase()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP8CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}

void AP8CharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AP8CharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

