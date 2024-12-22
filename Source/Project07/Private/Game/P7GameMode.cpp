// Copyright (C) 2023, IKinder

#include "Game/P7GameMode.h"

#include "Player/P7Character.h"
#include "Widget/P7HUD.h"

AP7GameMode::AP7GameMode()
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultPawnClass = AP7Character::StaticClass();
	HUDClass = AP7HUD::StaticClass();
}

void AP7GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AP7GameMode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

