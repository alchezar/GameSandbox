// Copyright (C) 2023, IKinder

#include "STU_RespawnComponent.h"
#include "STU_GameModeBase.h"

USTU_RespawnComponent::USTU_RespawnComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void USTU_RespawnComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USTU_RespawnComponent::Respawn(int32 RespawnTime)
{
	if (!GetWorld()) return;

	RespawnCountDown = RespawnTime;
	GetWorld()->GetTimerManager().SetTimer(RespawnTimer, this, &ThisClass::RespawnTimerUpdate, true, 1.f);
}

void USTU_RespawnComponent::RespawnTimerUpdate()
{
	if (--RespawnCountDown <= 0)
	{
		if (!GetWorld()) return;
		GetWorld()->GetTimerManager().ClearTimer(RespawnTimer);

		ASTU_GameModeBase* GameMode = Cast<ASTU_GameModeBase>(GetWorld()->GetAuthGameMode());
		if (!GameMode) return;

		GameMode->RespawnRequest(Cast<AController>(GetOwner()));
	}
}

int32 USTU_RespawnComponent::GetRespawnCountDown() const
{
	return RespawnCountDown;
}

bool USTU_RespawnComponent::IsRespawnInProgress() const
{
	return GetWorld() && GetWorld()->GetTimerManager().IsTimerActive(RespawnTimer);
}
