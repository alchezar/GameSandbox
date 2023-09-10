// Copyright (C) 2023, IKinder

#include "P10/Public/Game/P10GameState.h"

#include "Engine/PawnIterator.h"
#include "Net/UnrealNetwork.h"
#include "P10/Public/Player/P10PlayerController.h"

void AP10GameState::Multicast_OnMissionComplete_Implementation(APawn* InstigatorPawn, const bool bSuccess, const float ShowTime)
{
	for (auto It = TActorIterator<APlayerController>(GetWorld(), APlayerController::StaticClass()); It; ++It)
	{
		AP10PlayerController* Controller = Cast<AP10PlayerController>(*It);
		if (!Controller || !Controller->IsLocalController()) continue;
		APawn* Pawn = Controller->GetPawn();
		if (!Pawn || !Pawn->IsLocallyControlled()) continue;

		Pawn->DisableInput(Controller);
		Controller->OnMissionCompleted(InstigatorPawn, bSuccess, ShowTime);
	}
}

void AP10GameState::SetWaveState(const EP10WaveState NewState)
{
	const EP10WaveState OldState = WaveState;
	if (HasAuthority())
	{
		WaveState = NewState;
		OnRep_WaveState(OldState);
	}
}

void AP10GameState::OnRep_WaveState(const EP10WaveState OldState)
{
	WaveStateChanged(WaveState, OldState);
}

void AP10GameState::WaveStateChanged(const EP10WaveState NewState, const EP10WaveState OldState)
{
	GEngine->AddOnScreenDebugMessage(1, 5.f, FColor::Orange, FString::Printf(TEXT("%s"), *UEnum::GetDisplayValueAsText(NewState).ToString()));
}

void AP10GameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ThisClass, WaveState)
}
