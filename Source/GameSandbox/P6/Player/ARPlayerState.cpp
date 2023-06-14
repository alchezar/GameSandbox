// Copyright (C) 2023, IKinder

#include "ARPlayerState.h"

AARPlayerState::AARPlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AARPlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void AARPlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 AARPlayerState::GetCredits() const
{
	return Credits;
}

void AARPlayerState::AddCredits(const int32 Delta)
{
	if (Delta < 0) return;

	Credits += Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
	
}

bool AARPlayerState::RemoveCredits(const int32 Delta)
{
	if (Delta > 0 || Delta > Credits) return false;
	
	Credits -= Delta;
	OnCreditsChanged.Broadcast(this, Credits, Delta);
	return true;
}

