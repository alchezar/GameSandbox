// Copyright (C) 2023, IKinder

#include "P10/Public/Game/P10GameMode.h"

#include "P10/Public/Game/P10GameState.h"
#include "P10/Public/Player/P10Character.h"
#include "P10/Public/Player/P10PlayerController.h"
#include "P10/Public/UI/P10HUD.h"

AP10GameMode::AP10GameMode()
{
	DefaultPawnClass = AP10Character::StaticClass();
	HUDClass = AP10HUD::StaticClass();
	GameStateClass = AP10GameState::StaticClass();
	PlayerControllerClass = AP10PlayerController::StaticClass();
}

void AP10GameMode::BeginPlay()
{
	Super::BeginPlay();
}

void AP10GameMode::CompleteMission(APawn* InstigatorPawn, bool bSuccess)
{
	if (!InstigatorPawn) return;

	FString PrintMsg = UEnum::GetValueAsString(InstigatorPawn->GetRemoteRole()).RightChop(5) + ": ";
	PrintMsg += bSuccess ? "MISSION COMPLETE" : "MISSION FAILED";
	const FColor MsgColor = bSuccess ? FColor::Green : FColor::Red;
	if (GEngine) GEngine->AddOnScreenDebugMessage(1, 10.f, MsgColor, PrintMsg);

	if (AP10GameState* CurrentGameState = GetGameState<AP10GameState>())
	{
		CurrentGameState->Multicast_OnMissionComplete(InstigatorPawn, bSuccess);
	}
}