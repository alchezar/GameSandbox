// Copyright (C) 2023, IKinder

#include "Player/STU_PlayerState.h"

ASTU_PlayerState::ASTU_PlayerState()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ASTU_PlayerState::BeginPlay()
{
	Super::BeginPlay();
}

void ASTU_PlayerState::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

int32 ASTU_PlayerState::GetTeamID() const
{
	return TeamID;
}

FLinearColor ASTU_PlayerState::GetTeamColor() const
{
	return TeamColor;
}

int32 ASTU_PlayerState::GetKillsNum() const
{
	return KillsNum;
}

int32 ASTU_PlayerState::GetDeathNum() const
{
	return DeathsNum;
}

void ASTU_PlayerState::SetTeamID(const int32 NewID)
{
	TeamID = NewID;
}

void ASTU_PlayerState::SetTeamColor(const FLinearColor& NewTeamColor)
{
	TeamColor = NewTeamColor;
}

void ASTU_PlayerState::SetBlasterColor(const FLinearColor& NewBlasterColor)
{
	BlasterColor = NewBlasterColor;
}

void ASTU_PlayerState::AddKill()
{
	++KillsNum;
}

void ASTU_PlayerState::AddDeath()
{
	++DeathsNum;
}

void ASTU_PlayerState::LogInfo()
{
	GEngine->AddOnScreenDebugMessage(
		-1, 5.f, FColor::Orange, FString::Printf(TEXT("TeamID: %i, Kills: %i, Death: %i"), TeamID, KillsNum, DeathsNum));;
}

FLinearColor ASTU_PlayerState::GetBlasterColor() const
{
	return BlasterColor;
}
