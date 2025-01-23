// Copyright (C) 2023, IKinder

#include "Player/P11PlayerState.h"

#include "Game/P11GameInstance.h"
#include "Game/P11GameModeBase.h"
#include "Game/P11SavePlayerInfo.h"
#include "Kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "UI/Chat/P11Chat.h"
#include "UI/Chat/P11ChatMessage.h"

void AP11PlayerState::BeginPlay()
{
	Super::BeginPlay();

	if (GetPlayerController() && GetPlayerController()->IsLocalController())
	{
		const UP11GameInstance* GameInstance = Cast<UP11GameInstance>(GetGameInstance());
		if (!GameInstance)
		{
			return;
		}
		const UP11SavePlayerInfo* SavedInfo = Cast<UP11SavePlayerInfo>(UGameplayStatics::LoadGameFromSlot(GameInstance->GetSlotName(), 0));
		if (!SavedInfo)
		{
			return;
		}
		Server_SendNameToSrv(SavedInfo->PlayerName);
	}
}

void AP11PlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, PlayerName);
	DOREPLIFETIME(ThisClass, bDead);
	DOREPLIFETIME(ThisClass, Kills);
	DOREPLIFETIME(ThisClass, Deaths);
}

void AP11PlayerState::AddKill()
{
	++Kills;
}

void AP11PlayerState::AddDeath()
{
	++Deaths;
	bDead = true;
}

void AP11PlayerState::Server_SendNameToSrv_Implementation(const FText& Name)
{
	PlayerName = Name;

	/* Show message in chat after player connection. */
	AP11GameModeBase* GameMode = GetWorld()->GetAuthGameMode<AP11GameModeBase>();
	if (!GameMode)
	{
		return;
	}
	GameMode->Server_SendToGameMode({Name.ToString(), "Connected to the server.", EP11MessageState::Login});
}

void AP11PlayerState::SetIsAlive()
{
	bDead = false;
}
