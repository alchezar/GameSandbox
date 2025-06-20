// Copyright © 2025, Ivan Kinder

#include "Player/P17CharacterBase.h"

AP17CharacterBase::AP17CharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	GetMesh()->bReceivesDecals = false;
}

void AP17CharacterBase::BeginPlay()
{
	Super::BeginPlay();
}
