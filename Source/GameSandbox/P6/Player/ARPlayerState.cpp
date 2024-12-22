// Copyright (C) 2023, IKinder

#include "ARPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "P6/Game/ARSaveGame.h"

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

void AARPlayerState::SavePlayerState(UARSaveGame* SaveObject)
{
	if (!SaveObject) return;

	SaveObject->Credits = Credits;
}

void AARPlayerState::LoadPlayerState(const UARSaveGame* SaveObject)
{
	if (!SaveObject) return;

	AddCredits(SaveObject->Credits);
}

void AARPlayerState::OnRep_Credits(const int32 OldCredits)
{
	OnCreditsChanged.Broadcast(this, Credits, Credits - OldCredits);
}

void AARPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, Credits);
}
