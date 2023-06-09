// Copyright (C) 2023, IKinder

#include "ARAICharacter.h"

AARAICharacter::AARAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARAICharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AARAICharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}