// Copyright (C) 2023, IKinder

#include "P7/Public/Game/P7GameMode.h"
#include "P7/Public/Player/P7Character.h"

AP7GameMode::AP7GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = AP7Character::StaticClass();
}

void AP7GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AP7GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

