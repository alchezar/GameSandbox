// Copyright (C) 2023, IKinder

#include "P11/Public/Game/P11GameModeBase.h"

#include "EngineUtils.h"
#include "GameFramework/PlayerStart.h"
#include "P11/Public/Game/P11GameState.h"
#include "P11/Public/Player/P11Character.h"
#include "P11/Public/Player/P11PlayerController.h"
#include "P11/Public/Player/P11PlayerState.h"
#include "P11/Public/UI/P11HUD.h"
#include "P11/Public/UI/Chat/P11Chat.h"
#include "P11/Public/UI/Chat/P11ChatMessage.h"

AP11GameModeBase::AP11GameModeBase()
{
	DefaultPawnClass = AP11Character::StaticClass();
	HUDClass = AP11HUD::StaticClass();
	PlayerControllerClass = AP11PlayerController::StaticClass();
	PlayerStateClass = AP11PlayerState::StaticClass();
	GameStateClass = AP11GameState::StaticClass();
}

void AP11GameModeBase::Respawn(AController* Controller)
{
	/* Destroy dead character mesh. */
	if (APawn* DeadPawn = Controller->GetPawn())
	{
		Controller->UnPossess();
		DeadPawn->SetLifeSpan(5.f);
	}

	/* Find random player start transform. */
	TArray<APlayerStart*> AllPlayerStarts;
	for(TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
	{
		APlayerStart* PlayerStart = *It;
		AllPlayerStarts.Add(PlayerStart);
	}
	const int32 RandPlayerStart = FMath::RandRange(0, AllPlayerStarts.Num() - 1);
	const FTransform SpawnTransform = AllPlayerStarts[RandPlayerStart]->GetActorTransform();

	/* Spawn new character. */
	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	if (AP11Character* NewChar = GetWorld()->SpawnActor<AP11Character>(RespawnCharClass, SpawnTransform, Params))
	{
		Controller->Possess(NewChar);
	}
}

void AP11GameModeBase::Server_SendToGameMode_Implementation(const FP11MessageInfo& MessageInfo)
{
	AP11GameState* P11GameState = GetGameState<AP11GameState>();
	if (!P11GameState)
	{
		return;
	}
	for (const APlayerState* PlayerState : P11GameState->PlayerArray)
	{
		if (!PlayerState->GetPlayerController())
		{
			return;
		}
		AP11PlayerController* PlayerController = Cast<AP11PlayerController>(PlayerState->GetPlayerController());
		if (!PlayerController)
		{
			return;
		}
		PlayerController->Client_MessageSendToGameMode(MessageInfo);
	}
}

void AP11GameModeBase::OnPostLogin(AController* NewPlayer)
{
	Super::OnPostLogin(NewPlayer);
	/** Here we can show message in the chat, that the player is "Connected to the server.".
	  *	But GameMode spawn earlier than PlayerState, which contains the player`s name.
	  *	So we will send the message from AP11PlayerState::Server_SendNameToSrv. */
}

void AP11GameModeBase::Logout(AController* Exiting)
{
	Super::Logout(Exiting);

	const AP11PlayerState* PlayerState = Exiting->GetPlayerState<AP11PlayerState>();
	if (!PlayerState)
	{
		return;
	}
	Server_SendToGameMode({PlayerState->GetThePlayerName().ToString(), "Disconnected from the server.", EP11MessageState::Logout});
}
