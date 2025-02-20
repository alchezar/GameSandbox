// Copyright © 2025, Ivan Kinder

#include "Character/P16Enemy.h"

AP16Enemy::AP16Enemy()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AP16Enemy::BeginPlay()
{
	Super::BeginPlay();
}

void AP16Enemy::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);
}
